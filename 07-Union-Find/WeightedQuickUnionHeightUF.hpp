class WeightedQuickUnionHeightUF {
private:
    int* id;
    int* height;
    int mCount;
    const int mSize;
public:
    WeightedQuickUnionHeightUF(int n) : id(new int[n]), height(new int[n]), mCount(n), mSize(n) {
        for (int i = 0; i < mSize; ++i) {
            id[i] = i;
            height[i] = 1;
        }
    }

    ~WeightedQuickUnionHeightUF() { 
        delete[] id;
        delete[] height;
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

        if (height[i] < height[j]) {
            id[i] = j;
        } else if (height[i] < height[j]) {
            id[j] = i;
        } else {
            id[j] = i;
            ++height[i];
        }
        --mCount;
    }
};