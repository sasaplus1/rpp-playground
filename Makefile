.DEFAULT_GOAL := all

SHELL := /bin/bash

makefile := $(abspath $(lastword $(MAKEFILE_LIST)))
makefile_dir := $(dir $(makefile))

root_dir := $(makefile_dir)
build_dir := $(makefile_dir)/build

.PHONY: all
all: ## output targets
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(makefile) | awk 'BEGIN { FS = ":.*?## " }; { printf "\033[36m%-30s\033[0m %s\n", $$1, $$2 }'

.PHONY: build
build: ## build program
	cd $(build_dir) && cmake .. && cmake --build .

.PHONY: clean
clean: ## remove build dir
	$(RM) -rf $(build_dir) && mkdir $(build_dir) && touch $(build_dir)/.gitkeep

.PHONY: docker
docker: image := sasaplus1/rpp-playground
docker: ## run development environment
	docker build -t $(image) .
	docker run --rm -it -v $(root_dir):/rpi $(image)

.PHONY: fresh
fresh: ## cleanup cmake cache
	cd $(build_dir) && cmake --fresh .
