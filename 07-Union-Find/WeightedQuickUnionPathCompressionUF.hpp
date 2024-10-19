class WeightedQuickUnionPathCompressionUF {
private:
    int* id;
    int* sz;
    int mCount;
    const int mSize;
public:
    WeightedQuickUnionPathCompressionUF(int n) : id(new int[n]), sz(new int[n]), mCount(n), mSize(n) {
        for (int i = 0; i < mSize; ++i) {
            id[i] = i;
            sz[i] = 1;
        }
    }

    ~WeightedQuickUnionPathCompressionUF() { 
        delete[] id;
        delete[] sz;
    }

    int count() const { return mCount; }

    bool connected(int p, int q) { return find(p) == find(q); }

    int find(int p) {
        int root = p;

        // find root
        while (root != id[root]) root = id[root];

        // attach elements from p to root to the root
        while (p != root) {
            int pNew = id[p];
            id[p] = root;
            p = pNew;
        }

        return root;
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
};