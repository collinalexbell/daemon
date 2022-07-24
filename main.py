import torchaudio
from speechbrain.pretrained import Tacotron2
from speechbrain.pretrained import HIFIGAN
import fileinput
import sys

# Intialize TTS (tacotron2) and Vocoder (HiFIGAN)
tacotron2 = Tacotron2.from_hparams(source="speechbrain/tts-tacotron2-ljspeech", savedir="tmpdir_tts")
hifi_gan = HIFIGAN.from_hparams(source="speechbrain/tts-hifigan-ljspeech", savedir="tmpdir_vocoder", run_opts={"device":"cuda"} )

count = 0
for line in sys.stdin:
    # Running the TTS
    mel_output, mel_length, alignment = tacotron2.encode_text(line)

    # Running Vocoder (spectrogram-to-waveform)
    waveforms = hifi_gan.decode_batch(mel_output)

    waveforms = waveforms.squeeze(1).cpu()

    # Save the waverform
    torchaudio.save(f'{count}.wav',waveforms, 22050)
    print(count)
    count += 1
    count %= 100
