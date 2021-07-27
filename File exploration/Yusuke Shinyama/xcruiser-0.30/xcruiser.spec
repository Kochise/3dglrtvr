# $Id: monafont.spec,v 1.3 2002/12/23 10:57:32 s42335 Exp $
Summary: XCruiser, a filesystem visualization utility
Name: xcruiser
Version: 0.30
Release: 1
Group: X11/Applications
Copyright: GNU General Public License
URL: http://xcruiser.sourceforge.net/
Source: http://prdownloads.sourceforge.net/xcruiser/xcruiser-0.30.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-root
Packager: yusuke@cs.nyu.edu
Obsoletes: xcruise

%description
XCruiser is a filesystem visualization
utility which compares a filesystem to a 3D-formed universe and
allows you to "cruise" within it.

%prep
rm -rf ${RPM_BUILD_ROOT}

%setup -q

%build
xmkmf -a
make

%install
rm -rf ${RPM_BUILD_ROOT}
make install DESTDIR="${RPM_BUILD_ROOT}"
make install.man DESTDIR="${RPM_BUILD_ROOT}"

%clean
rm -rf ${RPM_BUILD_ROOT}

%post

%postun

%files
%defattr(-, root, root)
/usr/X11R6/bin/xcruiser
/usr/X11R6/man/man1/xcruiser.1x.gz
%doc README
%doc README.jp
%doc CHANGES
%doc TODO

%changelog
* Mon Feb 3 2003 Yusuke Shinyama <yusuke@cs.nyu.edu>
- Initial RPM release.
