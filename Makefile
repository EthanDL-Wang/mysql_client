ARCH?=x86

#app
VERSION_NUMBER?=0.0.1
VERSION?=$(VERSION_NUMBER)


.PHONY: build-sample-app clean-img

build-sample-app:
	docker build \
		-f app/Dockerfile.$(ARCH) \
		-t ethandl/mysql_client:$(VERSION)-$(ARCH) \
		.


clean-img:
	docker rmi ethandl/mysql_client:$(VERSION)-$(ARCH)