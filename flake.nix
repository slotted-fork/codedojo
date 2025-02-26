{
  description = "CodeDojo";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";
  };
  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};

      version = "0.1.0";

      nativeBuildInputs = with pkgs; [
      ];

      buildInputs = with pkgs; [
      ];
    in
      {
        devShells.${system}.default = pkgs.mkShell {
          inherit buildInputs nativeBuildInputs version;

          packages = with pkgs; [
          ];
        };

        packages.${system}.default = pkgs.stdenv.mkDerivation {
          inherit buildInputs nativeBuildInputs version;

          pname = "codedojo";
          src = ./.;

          installPhase = ''
            runHook preInstall
            make PREFIX="$out" install
            runHook postInstall
          '';
        };
      };
}
