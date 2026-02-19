#!/bin/bash

ansible-playbook -i inventory.ini playbook.yaml --ask-become-pass