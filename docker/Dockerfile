FROM alpine:edge as nix

RUN apk update && apk add nix

ARG NIX_SUBSTITOR
ARG NIX_SIGNING_KEY
ARG NIX_DERIVATION

ENV NIX_SUBSTITOR=${NIX_SUBSTITOR}
ENV NIX_SIGNING_KEY=${NIX_SIGNING_KEY}
RUN echo "extra-substituters = ${NIX_SUBSTITOR}" >>/etc/nix/nix.conf
RUN echo "extra-trusted-public-keys = ${NIX_SIGNING_KEY}" >>/etc/nix/nix.conf

ENV NIX_DERIVATION=${NIX_DERIVATION}
RUN nix --extra-experimental-features 'nix-command flakes' build "${NIX_DERIVATION}"

FROM scratch

ENTRYPOINT [ "/result/bin/ambiled" ]

COPY --from=nix /nix/store /nix/store
COPY --from=nix /result /result
