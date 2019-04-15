import foo : "../examples/foo.fish"

(fun bar (int i, flt f) (flt) {return 10.0})

start {
  dec flt f = 10.0
  dec [int] is = []
  is = [1,2]
}
