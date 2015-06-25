Configuration files
===================

All settings of Lensed, from the input files to the physical model of the lens
system, can be controlled from a single configuration file. A typical layout is
the following:

```ini
image       = double-lens.fits
gain        = 2400
offset      = 2.9633

output      = true
root        = chains/double-lens-

[objects]
lens        = sie
source      = sersic

[priors]
lens.x      = unif 48 52
lens.y      = unif 48 52
lens.r      = unif 15 20
lens.q      = unif 0 1
lens.pa     = unif 0 180
source.x    = unif 45 55
source.y    = unif 45 55
source.r    = unif 1 5
source.mag  = unif -5 0
source.n    = unif 0.5 8.0
source.q    = unif 0 1
source.pa   = unif 0 180

[labels]
lens.x      = x_L
lens.y      = y_L
lens.r      = r_L
lens.q      = q_L
lens.pa     = \theta_L
source.x    = x_S
source.y    = y_S
source.r    = r_S
source.mag  = mag_S
source.n    = n_S
source.q    = q_S
source.pa   = \theta_S
```

There are four sections in a typical configuration file: program options (top),
the definition of the objects in the lens system, priors for the individual
parameters of the model, and optionally a number of labels for output.


Options
-------

Options can be given in three places: as command line arguments, at the
top of the configuration file, or later in a group called `[options]`.

The following options are known to Lensed (see `lensed --help`):

Option     | Type           | Description                            | Default
-----------|----------------|----------------------------------------|--------
`device`   | `string`       | [Select computation device.](#device)  | `auto`
`output`   | `bool`         | Output results.                        | `true`
`root`     | `string`       | Root element for all output paths.     | 
`image`    | `path`         | Input image, FITS file in counts/sec.  | 
`gain`     | `real`, `path` | [Conversion factor to counts.](#gain)  | 
`offset`   | `real`         | Subtracted flat-field offset.          | `0`
`weight`   | `path`         | Weight map in 1/(counts/sec)^2.        | `none`
`mask`     | `path`         | Input mask, FITS file.                 | `none`
`psf`      | `path`         | Point-spread function, FITS file.      | `none`
`nlive`    | `int`          | Number of live points.                 | `300`
`ins`      | `bool`         | Use importance nested sampling.        | `true`
`mmodal`   | `bool`         | Mode separation (if ins = false).      | `true`
`ceff`     | `bool`         | Constant efficiency mode.              | `true`
`acc`      | `real`         | Target acceptance rate.                | `0.05`
`tol`      | `real`         | Tolerance in log-evidence.             | `0.1`
`shf`      | `real`         | Shrinking factor.                      | `0.8`
`maxmodes` | `int`          | Maximum number of expected modes.      | `100`
`feedback` | `bool`         | Show feedback from MultiNest.          | `false`
`updint`   | `int`          | Update interval for output.            | `1000`
`seed`     | `int`          | Random number seed for sampling.       | `-1`
`resume`   | `bool`         | Resume from last checkpoint.           | `false`
`maxiter`  | `int`          | Maximum number of iterations.          | `0`

Options without a default value are required to be set in the configuration.

There are a number of caveats regarding the following options.

### device

The default option for `device` is `auto`. This option will select the first
GPU device, in case one exists. Alternatively, it will select the first device
found.

### gain

The effective gain can be given either as a real number, in which case it will
be applied uniformly for each pixel of the image, or the path to a FITS file
that contains the effective gain for each individual pixel. This can be, for
example, the `EXP` image extension of a file generated by MultiDrizzle.


Objects
-------

Objects are the individual components that create the physical model for the
reconstruction. Each object corresponds to a definition in the `kernel` folder
that lists its parameters and physical properties.

The model used for reconstruction is created by listing one or more objects in
the `[objects]` section of the configuration file, together with a unique name
for identification.

**Important:** The order of objects in the configuration file determines the
physical layout of the system. If a source is meant to be placed before/behind
a lens, it must appear in the list of objects above/below that lens.

Example:

```ini
[objects]
lens   = sis
source = sersic
```

This model contains a SIS lens called `lens` and a Sérsic source called
`source`. The source appears behind the lens and will thus be deflected by it.

```ini
[objects]
host   = sersic
lens   = sis
source = sersic
```

The same as above, but including a foreground Sérsic source hosted on the lens
plane. As the `host` object appears before the lens, it will not be deflected.


Priors
------

Priors for parameters are specified in section `[priors]` of a configuration
file. They are given in the format

```ini
[priors]
obj.param = [wrap] <prior> <arg0> <arg1> ...
```

An exception is the pseudo-prior that fixes the value of a parameter, which is
given simply as `<value>`, without any name.

The optional `wrap` prefix can be used to indicate that the parameter values
wrap around at the boundaries of the prior. This is useful to get the correct
distribution for cyclic parameters such as the orientation, where a mean value
of 170 ± 20 degrees falls out of the natural [0, 180] degree bounds.

The following priors are known:

Prior          | Description
---------------|----------------------------------------------------------------
`<value>`      | pseudo-prior that fixes the parameter to the given *value*
`unif <a> <b>` | uniform prior on the interval [*a*, *b*]
`norm <m> <s>` | normal prior with mean *m* and standard deviation *s*

Example:

```ini
[priors]

; parameter "x" of object "lens" is fixed to 100
lens.x = 100

; normal prior for parameter "y" of object "lens" with mean 100 and sigma 20
lens.y = norm 100 20

; uniform probability for parameter "r" of object "lens" in interval [10, 20]
lens.r = unif 10 20

; the orientation angle is between 0 and 180 degrees and wraps around
lens.pa = wrap unif 0 180
```


Labels
------

It is possible to attach labels to parameters for post-processing e.g. with
getdist. These labels are given in the `[labels]` section of a configuration
file. The format is

```ini
[labels]
obj.param = <label>
```

Example:

```ini
[labels]

; label parameter "x" of object "lens"
lens.x = x_L
```