// export definition
exp dec dictionary someDict <[ int : str ]> new {1 : "hello"}

dec list someList <[ flt ]> new [1.0]

fun foo
<- void
-> void
{}

Cat Foo {
    int i
}

Cfn Foo {
    fun bar
    <- int i
    -> flt f
    {
        return i + 10.0
    }
}

start {
    foo = 10 == 1 and 2 or !4 % 7

    if |10 - 11 != 2| {} else {}
}
