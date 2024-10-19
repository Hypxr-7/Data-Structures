class QuickUnionUF {
private:
    int* id;
    int mCount;
    const int mSize;
public:
    QuickUnionUF(int n) : id(new int[n]), mCount(n), mSize(n) {
        for (int i = 0; i < mSize; ++i) id[i] = i; 
    }

    ~QuickUnionUF() { delete[] id; }

    int count() const { return mCount; }

    bool connected(int p, int q) const { return find(p) == find(q); }

    int find(int p) const {
        while (p != id[p]) p = id[p];
        return p;
    }

    void merge(int p, int q) {
        int pRoot = find(p);
        int qRoot = find(q);

        if (pRoot == qRoot) return;

        id[pRoot] = qRoot;
        --mCount;
    }
};