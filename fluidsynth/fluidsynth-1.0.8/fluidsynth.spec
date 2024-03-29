
%define name    fluidsynth
%define version 1.0.8
%define release 1
%define prefix  /usr

Summary: A real-time software synthesizer based on SoundFont 2 specifications.
Name: %{name}
Version: %{version}
Release: %{release}
Prefix: %{prefix}
Copyright: LGPL
Group: Sound
Source: http://savannah.nongnu.org/download/fluid/stable.pkg/%{version}/fluidsynth-%{version}.tar.gz
URL: http://www.fluidsynth.org/
BuildRoot: /var/tmp/%{name}-%{version}

%description
FluidSynth is a real-time software synthesizer based on the SoundFont
2 specifications. FluidSynth can read MIDI events from MIDI input
devices and render them to audio devices using SoundFont files to
define the instrument sounds. It can also play MIDI files and supports
real time effect control via SoundFont modulators and MIDI
controls. FluidSynth can be interfaced to other programs in different
ways, including linking as a shared library.

%package devel
Summary: Libraries and includes to build FluidSynth into other applications
Group: Development/Libraries

%description devel
FluidSynth is a real-time software synthesizer based on the SoundFont
2 specifications. FluidSynth can read MIDI events from MIDI input
devices and render them to audio devices using SoundFont files to
define the instrument sounds. It can also play MIDI files and supports
real time effect control via SoundFont modulators and MIDI
controls. FluidSynth can be interfaced to other programs in different
ways, including linking as a shared library.

This package contains libraries and includes for building applications
with FluidSynth support.

%prep
%setup

%build
./configure --prefix=%{prefix}
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
make prefix=$RPM_BUILD_ROOT%{prefix} install

%clean
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi

%files
%defattr(-,root,root)
%doc AUTHORS COPYING ChangeLog NEWS README TODO
%{prefix}/bin/fluidsynth
%{prefix}/lib/libfluidsynth.so*
%{prefix}/man/man1/*

%files devel
%defattr(-,root,root)
%doc doc/example.c doc/example.sf2 doc/api doc/html/*
%{prefix}/lib/libfluidsynth.a
%{prefix}/lib/libfluidsynth.la
%{prefix}/lib/pkgconfig/fluidsynth.pc
%{prefix}/include/fluidsynth.h
%{prefix}/include/fluidsynth

%changelog
* Mon Aug 25 2003 Josh Green <jgreen@users.sourceforge.net>
- Created initial fluidsynth.spec.in
