#!/bin/bash

if diff -q $1 $2; then
        echo file $1 e $2 identici
else
        echo file $1 e $2 diversi
fi
