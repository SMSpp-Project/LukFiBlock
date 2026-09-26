# test

A tester for `LukFiBlock` that needs nothing but the core SMS++ library.

Each of the 26 test functions of the `LukFiBlock` is loaded from an in-memory
stream and checked on its own, with no `Solver` on it: the number of
`ColVariable` of the `Block` and of active `Variable` of the `LukFiFunction`
(the functions of fixed dimension keeping theirs whatever the stream asks),
the value at the starting point against the one computed by hand from the
definition, and the value at the optimum of the literature against the
tabulated f*, which no point of a convex function can go below. The
linearization is checked at the starting point and at random points, near it,
far from it and near the optimum: its directional derivative along random
directions has to match a central finite difference (at a kink, the
linearization of a convex function has to lie between the two one-sided
differences), the linearization of a convex function has to stay below the
function at all the other points, the constant has to give back the value, and
the `Range` and the `Subset` forms have to return the same coefficients.

The edges are checked too: an unknown function or a negative number of
variables refused by `load()`, a second `load()` refused, a function of free
dimension with no variables, the kinks where a subgradient has to be chosen,
the continuity of Lewis across its pieces, the linearizations by name refused,
a wrong index in a `Subset` refused and a `Range` clipped to the variables.
Last, a change of the values of the `ColVariable` and of the parameters of
MaxQR has to be seen by the next `compute()`, and the netCDF round trip of the
`LukFiBlock` has to give back the same function, the parameters that are not
at their default being written in the file.

The random points come from fixed seeds, and the checks count the failures
instead of calling `assert()`, so that they hold under `NDEBUG` as well. The
exit code is 0 when every check passes, printing `All tests passed!!`, and 1
otherwise. The `makefile` builds the executable including the `LukFiBlock`
module and the core SMS++ library.


## Authors

- **Donato Meoli**  
  Dipartimento di Informatica  
  Università di Pisa


## License

This code is provided free of charge under the [GNU Lesser General Public
License version 3.0](https://opensource.org/licenses/lgpl-3.0.html),
see the [LICENSE](../LICENSE) file for details.
