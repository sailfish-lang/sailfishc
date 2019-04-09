import foo : "foo.fish"
import bar : "bar.fish"

(fun some_function([int] i)(void){
   i = i + 1
   return i
})

start {
   dec str s = "Throw youresel"
   dec [int] i = [1,2,3]
   f.f = f...foo() + (5 * 11 ** (4  and 6))
   z = 7

   Tree (
      (|1 == 2| {
         z = 2
      })

       (|1 != 2 and (!true)| {
         z = new Foo { z: 1, f: "hello from the otherside" }
         --z
      })
   )
}