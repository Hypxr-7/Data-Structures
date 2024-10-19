#include <vector>

class WeightedQuickUnionRAUF {
private:
    std::vector<int> id;
    std::vector<int> sz;
    int mCount;

public:
    WeightedQuickUnionRAUF() : mCount(0) {}

    WeightedQuickUnionRAUF(int N) : id(N), sz(N, 1), mCount(N) {
        for (int i = 0; i < N; ++i) {
            id[i] = i;
        }
    }

    int count() const { return mCount; }

    bool connected(int p, int q) const { return find(p) == find(q); }

    int find(int p) const {
        while (p != id[p]) p = id[p];
        return p;
    }

    void merge(int p, int q) {
        int i = find(p);
        int j = find(q);
        
        if (i == j) return;

        if (sz[i] < sz[j]) {
            id[i] = j;
            sz[j] += sz[i];
        } else {
            id[j] = i;
            sz[i] += sz[j];
        }
        --mCount;
    }

    int newSite() {
        int siteId = id.size();
        id.push_back(siteId);
        sz.push_back(1);
        ++mCount;
        return siteId;
    }
};