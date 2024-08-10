.DEFAULT_GOAL := all

SHELL := /bin/bash

makefile := $(abspath $(lastword $(MAKEFILE_LIST)))
makefile_dir := $(dir $(makefile))

root := $(makefile_dir)

.PHONY: all
all: ## output targets
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(makefile) | awk 'BEGIN { FS = ":.*?## " }; { printf "\033[36m%-30s\033[0m %s\n", $$1, $$2 }'

.PHONY: docker
docker: image := sasaplus1/rpp-playground
docker: ## run development environment
	docker build -t $(image) .
	docker run --rm -it -v $(root):/rpi $(image)
