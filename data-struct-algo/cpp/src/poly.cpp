#include <cstdio>
#include <vector>

struct Term {
    int coef;
    int exp;
};

using Poly = std::vector<Term>;

Poly scan_poly() {
    int n;
    scanf("%d", &n);

    Poly res(n);
    for (int i = 0; i < n; i++) {
        scanf("%d%d", &res[i].coef, &res[i].exp);
    }
    return res;
}

Poly add_poly(const Poly &pa, const Poly &pb) {
    Poly res;
    size_t i = 0, j = 0;
    while (i < pa.size() && j < pb.size()) {
        Term a = pa[i], b = pb[j];
        if (a.exp > b.exp) {
            res.push_back(a);
            i++;
        } else if (a.exp < b.exp) {
            res.push_back(b);
            j++;
        } else {
            a.coef += b.coef;
            if (a.coef != 0)
                res.push_back(a);
            i++;
            j++;
        }
    }
    while (i < pa.size())
        res.push_back(pa[i++]);
    while (j < pb.size())
        res.push_back(pb[j++]);
    return res;
}

int main() {
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        Poly a = scan_poly(), b = scan_poly();
        Poly res = add_poly(a, b);

        printf("%llu\n", res.size());
        for (Term term : res) {
            printf("%d %d\n", term.coef, term.exp);
        }
    }
    return 0;
}