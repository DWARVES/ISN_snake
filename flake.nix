
{
  description = "Multiplayer snake game";

  inputs = {
    nixpkgs.url = github:NixOS/nixpkgs/nixos-20.09;
  };

  outputs = { self, nixpkgs }:
    let
      system           = "x86_64-linux";
      pkgs             = nixpkgs.legacyPackages.${system};
      git-ignore       = pkgs.nix-gitignore.gitignoreSourcePure;
      pkg              = pkgs.stdenv.mkDerivation {
        name = "SnakekanS";
        src = git-ignore [ ./.gitignore ] ./.;
        buildInputs = with pkgs; [ cmake gnumake boost SDL SDL_ttf SDL_mixer SDL_image ];
        installPhase = ''
          mkdir -p $out/bin
          cp bin/Release/snakekans $out/bin
        '';
      };
    in {
      packages.${system} = {
        snakekans = pkg;
      };
      defaultPackage.${system} = self.packages.${system}.snakekans;
    };
}

