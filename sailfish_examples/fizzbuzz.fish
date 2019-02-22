fun fizzbuzz
<- int num
-> void
{
    if (num == 0) {
        return
    }

    fizzbuzzhelper(num)
    fizzbuzz(num--)
}

fun fizzbuzzhelper 
<- int num
-> void
{
        if   (i % 3 == 0)  { display ("Fizz") }
        else { 
            if (i % 5 == 0)  { display ("Buzz") }
            else {
                if (i % 15 == 0) { display ("FizzBuzz") }
                else {
                    display (i)
                }
            }
        }
}

start {
    fizzbuzz(100)
}
