# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.27
# 

Name:       harbour-linksbag

# >> macros
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    GetPocket client for SailfishOS
Version:    1.2
Release:    1
Group:      Applications/Network
License:    The MIT License (MIT)
URL:        https://github.com/Maledictus/LinksBag
Source0:    %{name}-%{version}.tar.bz2
Source100:  harbour-linksbag.yaml

Requires:   sailfishsilica-qt5 >= 0.10.9
Requires:   nemo-transferengine-qt5 declarative-transferengine-qt5 >= 0.0.44
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5WebKit)
BuildRequires:  pkgconfig(sailfishapp) >= 0.0.13
BuildRequires:  pkgconfig(nemotransferengine-qt5)
BuildRequires:  desktop-file-utils

%description
Application gives access to bookmarks stored in getpocket.com service

%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qtc_qmake5 

%qtc_make %{?_smp_mflags}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake5_install

# >> install post
# << install post

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}
%{_datadir}/%{name}/qml
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/86x86/apps/%{name}.png
%{_datadir}/%{name}
%{_datadir}/applications
%{_datadir}/icons/hicolor/86x86/apps
/usr/lib/nemo-transferengine/plugins/*.so
%attr(4755, root, root) %{_bindir}/*
# >> files
# << files

