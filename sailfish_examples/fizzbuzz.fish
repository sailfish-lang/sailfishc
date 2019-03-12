fun fizzbuzzhelper
<- int i
-> void
{
    dec int a = i % 15
    dec int b = i % 3
    dec int c = i % 5

    if | a == 0 | { display_str("FizzBuzz\n") }
    else {
        if | b == 0 | { display_str("Fizz\n") }
        else {
            if | c == 0 | { display_str("Buzz\n") }
            else {
                display_int(i)
                display_str("\n")
            }
        }
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