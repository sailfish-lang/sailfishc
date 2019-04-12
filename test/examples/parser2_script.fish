(fun foo (int i, flt f)(void){ return i })

start {
   Tree (
      (|1 == 2 and (2 < 3)| {})
      (|!true| {})
   )

   dec flt f = 12.0
   dec [int] is = [1,2,3]
   dec Foo f = new Foo {f: i}
   f.f += f...f() + 1
   ++f.f
}