# NixOS Configuration for HyperXAlpha

This folder contains NixOS-specific configuration for building and installing **HyperXAlpha**.  
These configurations are **not intended for use with Nix flakes** — they are standard Nix expressions compatible with traditional `nixpkgs` setups.

---

## Included Files

- `hyperxalpha.nix` – Nix derivation for building HyperXAlpha on NixOS.
- `README.md` – This file.

---

## Usage

### Adding to your `configuration.nix`

To include HyperXAlpha in your system environment, add the following to your `configuration.nix`:

```nix
environment.systemPackages = with pkgs; [
  # HyperXAlpha
  (callPackage ./pkgs/hyperxalpha.nix { })
];
