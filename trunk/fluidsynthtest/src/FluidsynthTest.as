/* Flash Midi Player - A midi player for the Adobe Flash Player
 *
 * Copyright (C) 2009 Yoann Huiban, yoann.huiban@gmail.com.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */
package {
	import cmodule.fluidsynth_swc.CLibInit;
	import flash.net.URLRequest;
	import flash.net.URLStream;
	import flash.utils.ByteArray;
	import flash.events.Event;
	import flash.events.SampleDataEvent;
	import flash.media.Sound;
	import flash.media.SoundChannel;
	import flash.utils.Endian;
   
	public class FluidsynthTest	{
		private var _urlStream:URLStream;
		private var _sf2FileData:ByteArray;
		private var _cLibInit:CLibInit;
		private var _clib:Object;
 		private var _sound:Sound=null;
        private var channel:SoundChannel;
		
		public function FluidsynthTest() {
			_cLibInit = new CLibInit;
			_urlStream = new URLStream();
			_sf2FileData = new ByteArray();
			_urlStream.addEventListener(Event.COMPLETE, completeHandler);
			_urlStream.load(new URLRequest("./example.sf2"));	
		}
		
		private function completeHandler(event:Event):void {
			var fileData:ByteArray = new ByteArray();
			var rawSampleData:ByteArray = new ByteArray();
			_urlStream.readBytes(_sf2FileData, 0, _urlStream.bytesAvailable);
			_clib = _cLibInit.init();
			_cLibInit.supplyFile("./example.sf2", _sf2FileData);
			_clib.fluidsynth_init(0,0);
			_clib.fluidsynth_noteon(0,62,80);
			start();
			
			/*_clib.fluidsynth_noteoff(0,70,80);
			_clib.fluidsynth_noteon(0,75,80);*/
			
			//trace(_clib.fluidsynth_cleanup());
		}

		public function start():void {
			stop();
			_sound = new Sound();
			_sound.addEventListener(SampleDataEvent.SAMPLE_DATA,samplesGenerator);
			channel = _sound.play();
			channel.addEventListener(Event.SOUND_COMPLETE, eventCompleteHandler);
		}
		
		public function stop():void {
			if (_sound) {
				_sound.removeEventListener(SampleDataEvent.SAMPLE_DATA, samplesGenerator);
				channel.removeEventListener(Event.SOUND_COMPLETE , eventCompleteHandler);
				_sound = null;
			}
		}
		
		private function samplesGenerator(event:SampleDataEvent):void {
			var rawSampleData:ByteArray = new ByteArray();
			
			rawSampleData.endian = Endian.LITTLE_ENDIAN;
			
			//trace("sineWavGenerator:"+event.position);
			_clib.fluidsynth_getdata(rawSampleData);
			rawSampleData.position=0;
			//trace("rawSampleData:"+rawSampleData.toString());
			//trace("rawSampleData.bytesAvailable:"+rawSampleData.bytesAvailable);
    		for ( var c:int=0; c<2048; c++ ) {
				var sample:Number = rawSampleData.readFloat();
				// left channel
				event.data.writeFloat(sample);
				// right channel
				event.data.writeFloat(sample);
			}
  		}
  		
		private function eventCompleteHandler(event:Event):void {
			trace("eventCompleteHandler");
  		}
	}
}
