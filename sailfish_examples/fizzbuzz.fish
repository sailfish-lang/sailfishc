fun fizzbuzzhelper
<- int i
-> void
{
    if | 0 == i % 15 | { display_str("FizzBuzz\n") }
    else if | 0 == i % 3 | { display_str("Fizz\n") }
    else if | 0 == i % 5 | { display_str("Buzz\n") }
    else {
        display_int(i)
        display_str("\n")
    }
}

fun fizzbuzz
<- int i
-> void
{
    fizzbuzzhelper(i)
    i = i - 1
    if |i > 0| { fizzbuzz(i) }
    else {}
}

start {
    fizzbuzz(100)
}