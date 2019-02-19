Cat Foo {
    int baz
    flt saz
    str zaz
}

Cfn Foo {
    fun bar 
    <- flt num, str word
    -> void
    {
    }
}

fun fizzbuzz 
<- int num, int num2, int num3
-> bool, str
{
  
}

start {
    loop i := [1,100] {
        fizzbuzz(i)
    }
}
