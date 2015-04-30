Summary: Assistor
Name: assistor
Version: 1.0
Release: 3
License: GPL
Group: System
Source: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires:automake libcurl-devel gtk2-devel
Packager: wen.xue
Provides: config(%{name}) = %{version}-%{release}
AutoProv: no
Prefix: /opt

%description
assistor 

%prep
%setup -q -n assistor

%build
make %{?_smp_mflags}


%install
rm -rf %{buildroot}
#%make install DESTDIR=%{buildroot}
mkdir -p %{buildroot}/usr/local/bin
mkdir -p %{buildroot}/usr/share/applications
mkdir -p %{buildroot}/usr/share/assistor/pixmaps
install -m 755 %{_builddir}/assistor/src/assistor %{buildroot}/usr/local/bin
install -m 555 %{_builddir}/assistor/desktop/assistor.desktop %{buildroot}/usr/share/applications
install -m 555 %{_builddir}/assistor/pixmaps/assistor.png %{buildroot}/usr/share/assistor/pixmaps
echo %{buildroot}
%post
%{__rm} -rf %{buildroot}

%files
%defattr(-,root,root,0755)

/usr/local/bin/assistor
/usr/share/applications/assistor.desktop
/usr/share/assistor/pixmaps/assistor.png
%changelog
* Wed Apr 29 2015 NeoKylin Linux - 1.0-3
- rebuilt


