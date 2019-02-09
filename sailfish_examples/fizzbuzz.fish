fun fizzbuzz 
<- int num
-> void
{
        if   (i % 3 == 0)  { display ("Fizz") }
        elif (i % 5 == 0)  { display ("Buzz") }
        elif (i % 15 == 0) { display ("FizzBuzz") }
        else               { display (i) }
}

start {
    /* 
        starts with 1, ends with 100; both sides are inclusive
        since we are using the common mathematical bracketing
        syntax. Thus (1,100) is exclusize on both sides
    */ 
    loop i := [1,100] {
        fizzbuzz(i)
    }
}
