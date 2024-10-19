class QuickFindUF
{
private:
    int *id;
    int mCount;
    const int mSize;

public:
    QuickFindUF(int n) : id(new int[n]), mCount(n), mSize(n)
    {
        for (int i = 0; i < mSize; ++i)
            id[i] = i;
    }

    ~QuickFindUF() { delete[] id; }

    int count() const { return mCount; }

    bool connected(int p, int q) const { return find(p) == find(q); }

    int find(int p) const
    {
        return id[p];
    }

    void merge(int p, int q)
    {
        int pID = find(p);
        int qID = find(q);

        if (pID == qID)
            return;

        for (int i = 0; i < mSize; ++i)
            if (id[i] == pID)
                id[i] = qID;
        --mCount;
    }
};