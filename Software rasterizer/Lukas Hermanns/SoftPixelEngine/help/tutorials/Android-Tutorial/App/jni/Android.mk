# NDK Makefile for "SPE Android Turorial" App

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE			:= Android-Tutorial
LOCAL_SRC_FILES			:= libAndroid-Tutorial.so

include $(PREBUILT_SHARED_LIBRARY)
