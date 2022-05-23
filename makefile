# Locate directories
class_d=bin
lib_d=libs
source_d=src
package_d=

# Compilation stuff
JAVAC=javac
JFLAGS=-g -d $(class_d) -sourcepath $(source_d) -Xlint:all

classpath:=$(class_d):$(lib_d)/gson-2.8.5.jar:$(lib_d)/http-2.2.1.jar:$(lib_d)/sqlite-jdbc-3.8.6.jar

# If there's already a CLASSPATH, put it on the front
ifneq ($(CLASSPATH),)
        classpath:= $(CLASSPATH):$(classpath)
endif


# Re-export the CLASSPATH.
export CLASSPATH:=$(classpath)

MATCH='.*[^(_TEST)]\.java'

ifdef TEST
    ifeq ($(TEST), all)
        MATCH='.*_TEST\.java'
    else
        MATCH='.*\/$(TEST)\/.*_TEST\.java'
    endif
endif

# Find all the source and convert them to class files
S_SERVER= $(shell find $(source_d)/server)
#-regex $(MATCH))
C_SERVER= $(patsubst src/%.java, bin/%.class, $(S_SERVER))
S_HANDLEDATA= $(shell find $(source_d)/handleData -regex $(MATCH))
C_HANDLEDATA= $(patsubst src/%.java, bin/%.class, $(S_HANDLEDATA))
S_HANDLERS= $(shell find $(source_d)/handlers -regex $(MATCH))
C_HANDLERS= $(patsubst src/%.java, bin/%.class, $(S_HANDLERS))
S_MODELS= $(shell find $(source_d)/models -regex $(MATCH))
C_MODELS= $(patsubst src/%.java, bin/%.class, $(S_MODELS))
S_REQUESTS= $(shell find $(source_d)/requests -regex $(MATCH))
C_REQUESTS= $(patsubst src/%.java, bin/%.class, $(S_REQUESTS))
S_REPONSES= $(shell find $(source_d)/responses -regex $(MATCH))
C_REPONSES= $(patsubst src/%.java, bin/%.class, $(S_REPONSES))

.SUFFIXES: .java .class
.PHONY: default server handleData handlers models requests responses clean init all

default: classes

$(class_d)/%.class: $(source_d)/%.java
	@echo "JAVAC $<"
	@$(JAVAC) $(JFLAGS) $<

classes: server handleData handlers models requests responses $(class_d)

server: $(C_SERVER) handleData handlers
handleData: $(C_HANDLEDATA)
handlers: $(C_HANDLERS) models requests responses
models: $(C_MODELSs)
requests: $(C_REQUESTS)
responses: $(C_REPONSES)

all: init classes

$(class_d):
	mkdir $(class_d)

clean:
	rm -rf $(class_d)/*
