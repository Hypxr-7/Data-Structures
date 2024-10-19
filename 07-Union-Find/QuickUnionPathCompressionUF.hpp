class QuickUnionPathCompressionUF {
private:
    int* id;
    int mCount;
    const int mSize;
public:
    QuickUnionPathCompressionUF(int n) : id(new int[n]), mCount(n), mSize(n) {
        for (int i = 0; i < mSize; ++i) id[i] = i; 
    }

    ~QuickUnionPathCompressionUF() { delete[] id; }

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
        int pRoot = find(p);
        int qRoot = find(q);

        if (pRoot == qRoot) return;

        id[pRoot] = qRoot;
        --mCount;
    }
};