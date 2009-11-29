/*
 * gcc fluidsynth_swc.c -O3 -I$ALCHEMY_HOME/fluidsynth/include -Wall -swc -o fluidsynth_swc.swc `pkg-config fluidsynth --libs`
 * gcc fluidsynth_swc.c -O3 -Wall -static -I$ALCHEMY_HOME/fluidsynth/include -L$ALCHEMY_HOME/fluidsynth/lib -lfluidsynth -swc -o fluidsynth_swc.swc 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//Header file for AS3 interop APIs
//this is linked in by the compiler (when using flaccon)
#include "AS3.h"

#include "fluidsynth.h"
#include "fluidsynth/settings.h"

fluid_synth_t* synth;
fluid_settings_t* settings;

static AS3_Val fluidsynth_init(void* self, AS3_Val args) {
	  int sfont_id;
	  int sfont_bank;
	  int sfont_preset;
	AS3_ArrayValue(args, "IntType,IntType", &sfont_bank,&sfont_preset);
	  /* Create the settings. */
	  settings = new_fluid_settings();
	  /* Create the synthesizer. */
	  synth = new_fluid_synth(settings);
	  sfont_id = fluid_synth_sfload(synth, "./example.sf2", 0);
	  /* Select bank 0 and preset 0 in the SoundFont we just loaded on
	     channel 0 */
	  fluid_synth_program_select(synth, 0, sfont_id, sfont_bank, sfont_preset); 
	  return AS3_Null();
}

static AS3_Val fluidsynth_cleanup(void* self, AS3_Val args) {
	  /* Clean up */
	  delete_fluid_synth(synth);
	  delete_fluid_settings(settings);
	  return AS3_Null();
}
static AS3_Val fluidsynth_noteon(void* self, AS3_Val args) {
	unsigned int channel;
	unsigned int key;
	unsigned int velocity;
	int res;
	AS3_ArrayValue(args, "IntType,IntType,IntType", &channel,&key,&velocity);
	res=fluid_synth_noteon(synth, channel, key, velocity);
	fluid_log(FLUID_WARN, "fluid_synth_noteon(%d,%d,%d)=%d",channel,key,velocity,res);
	return AS3_Null();
}
static AS3_Val fluidsynth_noteoff(void* self, AS3_Val args) {
	unsigned int channel;
	unsigned int key;
	int res;
	AS3_ArrayValue(args, "IntType,IntType", &channel,&key);
	res=fluid_synth_noteoff(synth, channel, key);
	fluid_log(FLUID_WARN, "fluid_synth_noteoff(%d,%d)=%d",channel,key,res);
	return AS3_Null();
}

static AS3_Val fluidsynth_allnotesoff(void* self, AS3_Val args) {
	unsigned int channel;
	int res;
	AS3_ArrayValue(args, "IntType", &channel);
	res=fluid_synth_all_notes_off(synth, channel);
	fluid_log(FLUID_WARN, "fluid_synth_all_notes_off(%d)=%d",channel,res);
	return AS3_Null();
}
// Control change
static AS3_Val fluidsynth_cc(void* self, AS3_Val args) {
	unsigned int channel;
	unsigned int num;
	unsigned int val;
	int res;
	AS3_ArrayValue(args, "IntType,IntType,IntType", &channel,&num,&val);
	res=fluid_synth_cc(synth, channel, num, val);
	return AS3_Null();
}
static AS3_Val fluidsynth_pitch_bend(void* self, AS3_Val args) {
	unsigned int channel;
	unsigned int val;
	int res;
	AS3_ArrayValue(args, "IntType,IntType", &channel,&val);
	res=fluid_synth_pitch_bend(synth, channel, val);
	return AS3_Null();
}
static AS3_Val fluidsynth_pitch_wheel_sens(void* self, AS3_Val args) {
	unsigned int channel;
	unsigned int val;
	int res;
	AS3_ArrayValue(args, "IntType,IntType", &channel,&val);
	res=fluid_synth_pitch_wheel_sens(synth, channel, val);
	return AS3_Null();
}
static AS3_Val fluidsynth_program_change(void* self, AS3_Val args) {
	unsigned int channel;
	unsigned int prognum;
	int res;
	AS3_ArrayValue(args, "IntType,IntType", &channel,&prognum);
	res=fluid_synth_pitch_wheel_sens(synth, channel, prognum);
	return AS3_Null();
}
static AS3_Val fluidsynth_bank_select(void* self, AS3_Val args) {
	unsigned int channel;
	unsigned int bank;
	int res;
	AS3_ArrayValue(args, "IntType,IntType", &channel,&bank);
	res=fluid_synth_bank_select(synth, channel, bank);
	return AS3_Null();
}
static AS3_Val fluidsynth_sfont_select(void* self, AS3_Val args) {
	unsigned int channel;
	unsigned int sfont_id;
	int res;
	AS3_ArrayValue(args, "IntType,IntType", &channel,&sfont_id);
	res=fluid_synth_sfont_select(synth, channel, sfont_id);
	return AS3_Null();
}

static AS3_Val fluidsynth_getdata(void* self, AS3_Val args) {
	  /** Generate a number of samples. This function expects two floating
	   *  point buffers (left and right channel) that will be filled with
	   *  samples.
	   *
	   *  \param synth The synthesizer
	   *  \param len The number of samples to generate
	   *  \param lout The sample buffer for the left channel
	   *  \param loff The offset, in samples, in the left buffer where the writing pointer starts
	   *  \param lincr The increment, in samples, of the writing pointer in the left buffer 
	   *  \param rout The sample buffer for the right channel
	   *  \param roff The offset, in samples, in the right buffer where the writing pointer starts
	   *  \param rincr The increment, in samples, of the writing pointer in the right buffer 
	   *  \returns 0 if no error occured, non-zero otherwise
	   */

	/*FLUIDSYNTH_API int fluid_synth_write_float(fluid_synth_t* synth, int len, 
						 void* lout, int loff, int lincr, 
						 void* rout, int roff, int rincr);*/	
	float l_out[8192];
	float r_out[8192];
	int c;
	unsigned int buffer_size=2048;
	AS3_Val decodedDataVal = AS3_Undefined();
	AS3_ArrayValue(args, "AS3ValType,IntType", &decodedDataVal, &buffer_size);

	fluid_synth_write_float(synth,buffer_size,l_out,0,1,r_out,0,1);
	//for ( c=0; c<2048; c++ ) {
	//	l_out[c]=sin(c/M_PI_2)*0.25;
	//}

	/* write len bytes from memory pointed to by src into byteArrayDst */
	//int AS3_ByteArray_writeBytes(AS3_Val byteArrayDst, void *src, int len);
	AS3_ByteArray_writeBytes(decodedDataVal,l_out,buffer_size*4);
	
	/* release the AS3 values */
	AS3_Release(decodedDataVal);
	return AS3_Null();
}

// Test method exposed to ActionScript
static AS3_Val fluidsynthtest(void* self, AS3_Val args) {
	  int i, key;
	  for (i = 0; i < 12; i++) {
	    /* Generate a random key */
	    key = 60 + (int) (12.0f * rand() / (float) RAND_MAX);
	    /* Play a note */
	    fluid_synth_noteon(synth, 0, key, 80);
	    /* Sleep for 1 second */
	    sleep(1000);
	    fluid_log(FLUID_WARN, "playing");
	    /* Stop the note */
	   fluid_synth_noteoff(synth, 0, key);
	  }
	// YH this is for testing
	  
	//initialize string to null
	char* val = NULL;
	
	//parse the arguments. Expect 1.
	//pass in val to hold the first argument, which
	//should be a string
	AS3_ArrayValue( args, "StrType", &val );
	
	//if no argument is specified
	if(val == NULL)	{
		char* nullString = "null";
		//return the string "null"
		return AS3_String(nullString);
	}
	
	//otherwise, return the string that was passed in
	return AS3_String(val);
}

int main() {
	//define the methods exposed to ActionScript
	//typed as an ActionScript Function instance
	AS3_Val versionVal = AS3_String("0.9.0");
	AS3_Val fluidsynth_initMethod = AS3_Function( NULL, fluidsynth_init );
	AS3_Val fluidsynth_cleanupMethod = AS3_Function( NULL, fluidsynth_cleanup );
	AS3_Val fluidsynthtestMethod = AS3_Function( NULL, fluidsynthtest );
	AS3_Val fluidsynth_getdataMethod = AS3_Function( NULL, fluidsynth_getdata );
	AS3_Val fluidsynth_noteonMethod = AS3_Function( NULL, fluidsynth_noteon );
	AS3_Val fluidsynth_noteoffMethod = AS3_Function( NULL, fluidsynth_noteoff );
	AS3_Val fluidsynth_allnotesoffMethod = AS3_Function( NULL, fluidsynth_allnotesoff );
	AS3_Val fluidsynth_ccMethod = AS3_Function( NULL, fluidsynth_cc );
	AS3_Val fluidsynth_pitch_bendMethod = AS3_Function( NULL, fluidsynth_pitch_bend );
	AS3_Val fluidsynth_pitch_wheel_sensMethod = AS3_Function( NULL, fluidsynth_pitch_wheel_sens );
	AS3_Val fluidsynth_program_changeMethod = AS3_Function( NULL, fluidsynth_program_change );
	AS3_Val fluidsynth_bank_selectMethod = AS3_Function( NULL, fluidsynth_bank_select );
	AS3_Val fluidsynth_sfont_selectMethod = AS3_Function( NULL, fluidsynth_sfont_select );
	// construct an object that holds references to the functions
	AS3_Val result = AS3_Object("version:StrType", versionVal);
	AS3_SetS(result, "fluidsynth_init", fluidsynth_initMethod);
	AS3_SetS(result, "fluidsynth_cleanup", fluidsynth_cleanupMethod);
	AS3_SetS(result, "fluidsynthtest", fluidsynthtestMethod);
	AS3_SetS(result, "fluidsynth_getdata", fluidsynth_getdataMethod);
	AS3_SetS(result, "fluidsynth_noteon", fluidsynth_noteonMethod);
	AS3_SetS(result, "fluidsynth_noteoff", fluidsynth_noteoffMethod);
	AS3_SetS(result, "fluidsynth_allnotesoff", fluidsynth_allnotesoffMethod);
	AS3_SetS(result, "fluidsynth_cc", fluidsynth_ccMethod);
	AS3_SetS(result, "fluidsynth_pitch_bend", fluidsynth_pitch_bendMethod);
	AS3_SetS(result, "fluidsynth_pitch_wheel_sens", fluidsynth_pitch_wheel_sensMethod);
	AS3_SetS(result, "fluidsynth_program_change", fluidsynth_program_changeMethod);
	AS3_SetS(result, "fluidsynth_bank_select", fluidsynth_bank_selectMethod);
	AS3_SetS(result, "fluidsynth_sfont_select", fluidsynth_sfont_selectMethod);
	// Release
	AS3_Release( fluidsynth_initMethod );
	AS3_Release( fluidsynth_cleanupMethod );
	AS3_Release( fluidsynthtestMethod );
	AS3_Release( fluidsynth_getdataMethod );
	AS3_Release( fluidsynth_noteonMethod );
	AS3_Release( fluidsynth_noteoffMethod );
	AS3_Release( fluidsynth_allnotesoffMethod);
	AS3_Release( fluidsynth_ccMethod);
	AS3_Release( fluidsynth_pitch_bendMethod);
	AS3_Release( fluidsynth_pitch_wheel_sensMethod);
	AS3_Release( fluidsynth_program_changeMethod);
	AS3_Release( fluidsynth_bank_selectMethod);
	AS3_Release( fluidsynth_sfont_selectMethod);
	// notify that we initialized -- THIS DOES NOT RETURN!
	AS3_LibInit( result );
	// should never get here!
	return 0;

}
