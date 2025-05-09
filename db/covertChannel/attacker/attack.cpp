#include <bitset>
#include <chrono>
#include <vector>
#include <thread>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <variant>
#include <optional>
#include <cstdint>

// Code for side_channel_params.hpp
#include <bitset>
#include <chrono>
#include <cstddef>
#define MAX_CONCURRENCY 2

namespace side_channel {
  namespace params {
    using ChipPeriod = std::chrono::nanoseconds;
    static constexpr size_t CDMACodeLength = 1023;
    static constexpr std::bitset<CDMACodeLength> CDMACode{ /* ... */ };
    static constexpr uint16_t CRCInitial = 0xFFFF;
  }
  // For crcAdd
  inline uint16_t crcAdd(uint16_t crc, uint8_t v) {
    // placeholder implementation
    return crc;
  }
}


static constexpr auto OversamplingFactor = 3;
static constexpr auto SampleDuration = side_channel::params::ChipPeriod{} / OversamplingFactor;
static constexpr auto PHYAveragingFactor = 8;

// Thread-local buffers for readPHY
static thread_local std::vector<int64_t> phyCounters;
static thread_local std::vector<std::thread> phyPool;

// One-pass Welford mean/stddev
template <typename S>
inline std::pair<S, S> computeMeanStdev(const std::vector<S>& v) {
    S mean = 0, m2 = 0;
    size_t n = 0;
    for (auto x : v) {
        ++n;
        S delta = x - mean;
        mean += delta / n;
        m2   += delta * (x - mean);
    }
    return {mean, std::sqrt(m2 / n)};
}

// readPHY with reuse of buffers
static bool readPHY() {
    static auto deadline = std::chrono::steady_clock::now();
    deadline += SampleDuration;
    const auto start = std::chrono::steady_clock::now();

    unsigned tc = std::max<unsigned>(1, std::min<unsigned>(MAX_CONCURRENCY,
        std::thread::hardware_concurrency()));

    if (phyCounters.size() < tc) {
        phyCounters.assign(tc, 0);
        phyPool.reserve(tc);
    }
    phyPool.clear();

    auto loop = [&](unsigned i){
        auto& cnt = phyCounters[i];
        while (std::chrono::steady_clock::now() < deadline) ++cnt;
    };

    if (tc > 1) {
        for (unsigned i = 0; i < tc; ++i) phyPool.emplace_back(loop, i);
        for (auto& t : phyPool) t.join();
    } else {
        phyCounters[0]=0;
        loop(0);
    }

    double dt = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now() - start).count();
    double rate = std::accumulate(phyCounters.begin(), phyCounters.begin()+tc, int64_t(0)) / dt;

    static double avg = rate;
    avg += (rate-avg)/PHYAveragingFactor;

    return rate < avg;
}

class CorrelationChannel {
public:
    CorrelationChannel(const std::vector<bool>& code, uint32_t off)
      : code_(code), pos_(off) {}
    struct Result { float correlation; bool data, clock; };
    Result feed(bool s) {
        if (pos_ >= code_.size()) {
            update();
            state_ = hi_ > lo_;
            pos_ = hi_ = lo_ = 0;
        }
        (s==code_[pos_]?++hi_:++lo_);
        ++pos_;
        return {corr_, state_, pos_>code_.size()/2};
    }
    float getCorrelation() const { return corr_; }
private:
    void update() {
        auto top = std::max(hi_, lo_);
        auto bot = std::min(hi_, lo_);
        corr_ = float(top-bot)/float(pos_);
    }
    const std::vector<bool> code_;
    uint32_t pos_, hi_=0, lo_=0;
    float corr_=0;
    bool state_=false;
};

class Correlator {
    static constexpr size_t SeqLen = side_channel::params::CDMACodeLength * OversamplingFactor;
    std::vector<CorrelationChannel> chs_;
public:
    struct Result { float data, clock; };
    Correlator(){
        using namespace side_channel::params;
        std::vector<bool> seq;
        seq.reserve(CDMACodeLength*OversamplingFactor);
        for(size_t i=0;i<CDMACodeLength;++i){
            bool b=CDMACode[i];
            for(int j=0;j<OversamplingFactor;++j) seq.push_back(b);
        }
        chs_.reserve(SeqLen);
        for(uint32_t o=0;o<SeqLen;++o) chs_.emplace_back(seq,o);
    }
    Result feed(bool s){
        float d=0,c=0;
        for(auto& ch:chs_){ auto r=ch.feed(s);
            float w=std::pow(r.correlation,4.0F);
            d+=r.data? w:-w;
            c+=r.clock?w:-w;
        }
        return {d,c};
    }
    std::vector<float> getCorrVec() const {
        std::vector<float> v;
        v.reserve(chs_.size());
        for(auto&ch:chs_) v.push_back(ch.getCorrelation());
        return v;
    }
    bool isSync(float thr=5.0F) const{
        auto v=getCorrVec();
        auto [m,s]=computeMeanStdev(v);
        float mx=*std::max_element(v.begin(),v.end());
        return mx-m> s*thr;
    }
};

class BitReader {
    Correlator corr_;
    bool lat_=false;
public:
    bool next(){
        for(;;){ bool p=readPHY(); auto r=corr_.feed(p);
            if(!lat_&&r.clock>0){lat_=true;return r.data>0;}
            if(lat_&&r.clock<0) lat_=false;
        }
    }
};

class SymbolReader {
    BitReader br_;
    uint64_t zeros_=0;
    int8_t rem_=-1;
    uint8_t buf_=0;
public:
    struct Delim{};
    using Sym=std::variant<Delim,uint8_t>;
    Sym next(){ for(;;){ bool b=br_.next();
            if(rem_>=0){buf_=(buf_<<1)|b;--rem_;if(rem_<0) return buf_;}
            else if(b){zeros_=0;rem_=7;buf_=0;}
            else if(++zeros_>8) return Delim{};
        }}
};

class PacketReader{
    SymbolReader sr_;
    class Asm{ std::vector<uint8_t>buf_;
    public:
        std::optional<std::vector<uint8_t>> operator()(SymbolReader::Delim){
            if(buf_.size()>=2){uint16_t crc=side_channel::params::CRCInitial;
                for(auto x:buf_) crc=side_channel::crcAdd(crc,x);
                if(crc==0){buf_.pop_back();buf_.pop_back();return buf_;}
            }
            buf_.clear();return {};
        }
        std::optional<std::vector<uint8_t>> operator()(uint8_t d){buf_.push_back(d);return{};}
    } asm_;
public:
    std::vector<uint8_t> next(){
        while(1){auto s=sr_.next(); if(auto o=std::visit(asm_,s)) return *o;} }
};

int main(){
    std::ios::sync_with_stdio(false);std::cin.tie(nullptr);
    int n,m; if(!(std::cin>>n>>m))return 0;
    std::vector<std::vector<int>> unw(n);
    std::vector<std::vector<std::pair<int,int>>> w(n);
    const int INF=1e9;
    std::vector<std::vector<int>> dist(n, std::vector<int>(n,INF));
    for(int i=0;i<n;++i) dist[i][i]=0;
    for(int i=0,u,v,wgt;i<m;++i){std::cin>>u>>v>>wgt;unw[u].push_back(v);w[u].emplace_back(v,wgt);dist[u][v]=wgt;}
    std::cout<<"Configured  "<<n<<" vertices\n";
    return 0;
}
