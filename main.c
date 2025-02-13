// 1/27 lecture

int counter(){
    static int n = 0;
    n++;

    return n;
}

int main(int argc, char** argv){
    printf("%d\n",counter());

    // is this allowed?
    double x = 2.5;
    x = x + 1;
    // use of x here forces floating point arithmetic
    // 1 is automatically promoted to double

    #include <limits.h>

    int y = INT_MAX; // largest possible signed int

    y = y + 1; // undefined!!
    // it might be INT_MIN, but we can't assume that
    // C defines that overflow of unsigned integers doesn't happen --- what happens to y < y+1 then if y is unsigned int?

    if(y < y + 1){
        // compiler is allowed to assume this is always true
        // add u b san - undefined behavior sanitizer to be careful!!
    }
}