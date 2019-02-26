// export definition
exp dec dictionary someDict = new {1 : "hello"}

// general decleration
dec list someList = new [1, 2]

// function
fun someFunc
<- int i
-> void
{}

// user defined type
Cat Foo {
    str s
}
Cfn Foo {
    fun s
    <- void
    -> int
    {}
}

// initial execution body
start {
    dec flt someFlt = 10.0
    if |1 == 2 and true or '\n' > "Hello World" < 4 >= 5 <= 1 != 0| {
        someFlt = 10 + 1 - 7 % 2 * 5 / someList[1]
        return Foo.s
    } else {
        return Foo...s(someFlt)
    }
}

