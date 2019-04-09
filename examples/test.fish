import Foo : "foo.fish"

Uat {
    int i
    flt f
}

Ufn {
	(fun main(int i)(int){
                Tree ( 
                        (|1 + 2 == 11 - 12 | {
                                dec int i = 10
                                a = 12 + (i - 10)
                                return a
                        })
                )
        })
}
