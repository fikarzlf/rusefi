package com.rusefi.config.generated;

// this file was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/pid_state.txt Sun Sep 01 11:57:50 EDT 2019

// by class com.rusefi.output.JavaFieldsConsumer
import com.rusefi.config.*;

public class PidState {
	public static final Field ITERM = Field.create("ITERM", 0, FieldType.FLOAT);
	public static final Field DTERM = Field.create("DTERM", 4, FieldType.FLOAT);
	public static final Field TARGET = Field.create("TARGET", 8, FieldType.FLOAT);
	public static final Field INPUT = Field.create("INPUT", 12, FieldType.FLOAT);
	public static final Field OUTPUT = Field.create("OUTPUT", 16, FieldType.FLOAT);
	public static final Field ERRORAMPLIFICATIONCOEF = Field.create("ERRORAMPLIFICATIONCOEF", 20, FieldType.FLOAT);
	public static final Field[] VALUES = {
	ITERM,
	DTERM,
	TARGET,
	INPUT,
	OUTPUT,
	ERRORAMPLIFICATIONCOEF,
	};
}