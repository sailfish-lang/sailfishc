fun fizzbuzzhelper
<- int i
-> void
{
    dec int a = i
    a = i % 15 == 0

    if | a == 0 | { display_str("FizzBuzz") }
    else {
        if | a == 0 | { display_str("Fizz") }
        else {
            if | a == 0 | { display_str("Buzz") }
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