Uat {
    int i
    Foo s
}

Ufn {
    (fun bar(int i, int a)(int) {
        return (i + own.i + a)
    })

    (fun self(void)(Foo) {
        return own.s
    })
}