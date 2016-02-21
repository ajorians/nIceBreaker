Summary:	An addictive action-puzzle game involving bouncing penguins
Name:		icebreaker
Version:	1.2.1
Release:	1
Epoch:		3
Copyright:	GPL
Group:		Amusements/Games
Source: 	icebreaker-%{version}.tgz
URL:		http://www.mattdm.org/icebreaker/
Prefix:		%{_prefix}
Vendor: 	Matthew Miller <mattdm@mattdm.org>
Packager:	Matthew Miller <mattdm@mattdm.org>
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-buildroot
BuildRequires:  SDL-devel, SDL_mixer-devel, /bin/awk

%description
So, uh, there's a bunch of penguins on an iceberg in Antarctica. You have
been selected to catch them so they can be shipped to Finland, where they
are essential to a secret plot for world domination.

%prep
%setup -q

%build
make OPTIMIZE="$RPM_OPT_FLAGS -finline-functions" highscoredir=/var/lib/games prefix=/usr

%install
rm -rf $RPM_BUILD_ROOT

mkdir -p ${RPM_BUILD_ROOT}%{_bindir}
mkdir -p ${RPM_BUILD_ROOT}%{_datadir}/icebreaker
mkdir -p ${RPM_BUILD_ROOT}%{_var}/lib/games
mkdir -p ${RPM_BUILD_ROOT}/etc/X11/applnk/Games
mkdir -p ${RPM_BUILD_ROOT}%{_mandir}/man6

install -m 644 *.wav *.bmp ${RPM_BUILD_ROOT}%{_datadir}/icebreaker
install -m 644 icebreaker.desktop ${RPM_BUILD_ROOT}/etc/X11/applnk/Games
install -s -m 755 icebreaker ${RPM_BUILD_ROOT}%{_bindir}
install -m 644 icebreaker.6  ${RPM_BUILD_ROOT}%{_mandir}/man6

touch ${RPM_BUILD_ROOT}%{_var}/lib/games/icebreaker.scores

%post
touch %{_var}/lib/games/icebreaker.scores
chown games:games %{_var}/lib/games/icebreaker.scores
chmod 0664 %{_var}/lib/games/icebreaker.scores

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr (-,root,root)
%doc README TODO LICENSE ChangeLog
%attr(2755,root,games) %{_bindir}/icebreaker
/etc/X11/applnk/Games/icebreaker.desktop
%{_datadir}/icebreaker
%{_mandir}/man6/*
%attr(664,games,games) %ghost %{_var}/lib/games/icebreaker.scores

%changelog
* Sat Jul 28 2001 Matthew Miller <mattdm@mattdm.org>
- 1.2

* Tue Jul 24 2001 Matthew Miller <mattdm@mattdm.org>
- move man page section 6

* Sun Jul 22 2001 Matthew Miller <mattdm@mattdm.org>
- 1.1

* Fri Jul 20 2001 Matthew Miller <mattdm@mattdm.org>
- borrowed idea of using post-script to create high score file
  from Mandrake RPM. That way, it doesn't have to be marked as a config
  file, and yet won't get zapped on upgrade.
- also, modified Makefile to cope with RPM_OPT_FLAGS, again as per
  Mandrake.

* Thu Jul 19 2001 Matthew Miller <mattdm@mattdm.org>
- added man page

* Tue Jul 18 2001 Matthew Miller <mattdm@mattdm.org>
- updated to 1.09

* Thu Oct 5 2000 Matthew Miller <mattdm@mattdm.org>
- looks good to me. one-point-oh

* Tue Oct 3 2000 Matthew Miller <mattdm@mattdm.org>
- updated to 0.995 
- better make process

* Mon Oct 2 2000 Matthew Miller <mattdm@mattdm.org>
- updated to 0.99 :)

* Mon Oct 2 2000 Matthew Miller <mattdm@mattdm.org>
- updated to 0.98

* Fri Sep 15 2000 Matthew Miller <mattdm@mattdm.org>
- first package
