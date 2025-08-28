{ lib
, stdenv
, fetchFromGitHub
, cmake
, pkg-config
, systemd
, udev
, wxGTK32
, hidapi
}:

stdenv.mkDerivation rec {
  pname = "hyperxalpha";
  version = "1.0.0";

  src = fetchFromGitHub {
    owner = "Stroopwafe1";
    repo = "HyperXAlpha";
    rev = "d2d4d04";
    sha256 = "ylxz8kJA3cJW1hubmBXPzGzWuZJzmpaUblY8obh05g8=";
  };

  nativeBuildInputs = [ cmake pkg-config ];
  buildInputs = [ systemd udev wxGTK32 hidapi ];

  installPhase = ''
    mkdir -p $out/bin
    cp ../bin/HyperX $out/bin/
  '';

  meta = with lib; {
    description = "HyperX Alpha headset control software";
    homepage = "https://github.com/Stroopwafe1/HyperXAlpha";
    license = licenses.gpl3Plus;
    maintainers = with maintainers; [ ];
    platforms = platforms.linux;
  };
}
