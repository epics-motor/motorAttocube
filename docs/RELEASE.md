# motorAttocube Releases

## __R1-1 (2026-01-29)__
R1-1 is a release based on the master branch.

### Changes since R1-0-2

#### New features
* Pull request [#2](https://github.com/epics-motor/motorAttocube/pull/2): [Jakub Wlodek](https://github.com/jwlodek) added an asyn model-3 driver with support for Attocube AMC controllers like the AMC100 (requires EPICS base 7.0.3.1 or later)

#### Modifications to existing features
* None

#### Bug fixes
* Pull request [#4](https://github.com/epics-motor/motorAttocube/pull/4): Only build the new motorAttocubeDriver on Windows and Linux
* Pull request [#6](https://github.com/epics-motor/motorAttocube/pull/6): Bug fix for compile errors for custom linux EPICS_HOST_ARCHs

#### Continuous integration
* Upgraded ci-scripts (v3.4.1)

## __R1-0-2 (2023-04-11)__
R1-0-2 is a release based on the master branch.

### Changes since R1-0-1

#### New features
* None

#### Modifications to existing features
* None

#### Bug fixes
* None

#### Continuous integration
* Added ci-scripts (v3.0.1)
* Configured to use Github Actions for CI

## __R1-0-1 (2020-05-11)__
R1-0-1 is a release based on the master branch.  

### Changes since R1-0

#### New features
* None

#### Modifications to existing features
* None

#### Bug fixes
* Commit [331069c](https://github.com/epics-motor/motorAttocube/commit/331069c605e03cd78c438d53c1d8decb714f4d83): Include ``$(MOTOR)/modules/RELEASE.$(EPICS_HOST_ARCH).local`` instead of ``$(MOTOR)/configure/RELEASE``

## __R1-0 (2019-04-18)__
R1-0 is a release based on the master branch.  

### Changes since motor-6-11

motorAttocube is now a standalone module, as well as a submodule of [motor](https://github.com/epics-modules/motor)

#### New features
* motorAttocube can be built outside of the motor directory
* motorAttocube has a dedicated example IOC that can be built outside of motorAttocube

#### Modifications to existing features
* None

#### Bug fixes
* None

