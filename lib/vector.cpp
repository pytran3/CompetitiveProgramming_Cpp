#define all(x) (x).begin(),(x).end()


template<typename T>
struct CompressionVector {
    vector<T> v;
    CompressionVector(const vector<T> & v) noexcept {
      this->v = v;
      sort(all(this->v));
      this->v.erase(unique(all(this->v)), this->v.end());
    }

    int index_of(T x) {
      return lower_bound(all(v), x) - v.begin();
    }
};
