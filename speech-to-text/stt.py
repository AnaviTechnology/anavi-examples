import speech_recognition as sr
import whisper
import numpy as np
import sys

class WhisperRecognizer:
    def __init__(self, model_size="base"):
        self.model = whisper.load_model(model_size)
    
    def recognize(self, audio_data):
        # Convert the audio data to a NumPy array
        audio = np.frombuffer(audio_data.get_raw_data(), np.int16).astype(np.float32) / 32768.0

        # Use Whisper to transcribe the audio
        result = self.model.transcribe(audio, fp16=False)
        return result["text"]

def find_microphone(mic_name):
    """Find a microphone by name."""
    mic_list = sr.Microphone.list_microphone_names()
    for i, name in enumerate(mic_list):
        if mic_name in name:
            return i
    return None

def main():
    mic_name = "Dev Mic: USB Audio"
    
    # Find the microphone by name
    mic_index = find_microphone(mic_name)
    
    if mic_index is None:
        print("ANAVI Dev Mic not found.")
        sys.exit(1)

    try:
        mic = sr.Microphone(device_index=mic_index)
        recognizer = sr.Recognizer()
        whisper_recognizer = WhisperRecognizer(model_size="base")

        print("\nListening... Press Ctrl+C to stop.")

        with mic as source:
            recognizer.adjust_for_ambient_noise(source)
            while True:
                print("Listening...")
                audio = recognizer.listen(source)

                try:
                    text = whisper_recognizer.recognize(audio)
                    print(f"You said: {text}")
                except Exception as e:
                    print(f"Could not recognize speech; {e}")

    except KeyboardInterrupt:
        print("\nTerminating the program.")
        sys.exit(0)

if __name__ == "__main__":
    main()
