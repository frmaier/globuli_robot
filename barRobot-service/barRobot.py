#!/usr/bin/python
# coding=utf-8
"""
# coding=utf-8
Script wird von /etc/rc.local genutzt
sudo nano /etc/rc.local

systemctl daemon-reload  #immer starten wenn rc.local oder barRobot.py geändert wurde, startet script aber nicht neu (siehe nächste zeile)
systemctl restart rc-local # um dienst und damit das script  neu zu starten

ps -A | grep python3  # testen ob scrpt läuft
/tmp/barRobot.log  
tail /tmp/barRobot.log -f -n 10   # zeigt immer die letzten 10 aktuellen zeilen des logs
less /tmp/barRobot.log # anziegen log
https://gpiozero.readthedocs.io/en/stable/recipes.html <-dort auch die pinbelegung
"""

import random
import pygame
import wave
from gpiozero import LED, Button, OutputDevice
import time
import logging
from signal import pause
from gpiozero import MotionSensor

red1=LED(17)
red2=LED(27)
red3=LED(22)

red1.off()
red2.off()
red3.off()

button1=Button(16) #grün
button2=Button(20) # gelb
button3=Button(21) # orange
button4=Button(12) # lila 
buttonM=Button(5) #Muenzeinwurf
pir = MotionSensor(4)
isPumping=False

RELAY1_PIN = 18 #gelb
RELAY2_PIN = 23 #blau
RELAY3_PIN = 24 #lila
RELAY4_PIN = 25 #grau
relay1 = OutputDevice(RELAY1_PIN, active_high=False, initial_value=False)
relay2 = OutputDevice(RELAY2_PIN, active_high=False, initial_value=False)
relay3 = OutputDevice(RELAY3_PIN, active_high=False, initial_value=False)
relay4 = OutputDevice(RELAY4_PIN, active_high=False, initial_value=False)

_numM=0 #Anzahl der eingeworfenen M�nzen


#pygame.init()
SONG_END = pygame.USEREVENT + 1
pygame.mixer.music.set_endevent(SONG_END)
file_path = 'sound/muenze1.wav'
file_wav = wave.open(file_path)
freq = file_wav.getframerate()
print freq
pygame.mixer.quit
pygame.mixer.pre_init(frequency=freq)
pygame.init()
pygame.mixer.init()
#pygame.mixer.init(frequency=freq);

_songList1 = ['1.wav', '2.wav', '3.wav','4.wav']
_coinList = ['sound/Coins/131935__mcpable__coin-spinning.wav', 'sound/Coins/369701__mrguff__coin-drop-into-pot-2.wav','sound/Coins/67255__aent__coin-drop.wav','sound/Coins/341231__sharesynth__coin00.wav','sound/Coins/97373__thedjoe93__coin.wav','sound/Coins/420996__eponn__coin-2.wav']
_currently_playing_song = None

pygame.mixer.music.load("sound/muenze1.wav")
sd_muenze1=pygame.mixer.Sound('sound/muenze1.wav')
sd_muenze2=pygame.mixer.Sound('sound/muenze2.wav')
sd_cocktail=pygame.mixer.Sound('sound/cocktail.wav')
sd_button=pygame.mixer.Sound('sound/button.wav')
sound1=pygame.mixer.Sound('1.wav')
sd_coin1=pygame.mixer.Sound('sound/Coins/131935__mcpable__coin-spinning.wav')
###########
print("--------------")

def set_relay1(status):
    if status:
        logging.info("Setting relay1: ON")
        relay1.on()
    else:
        logging.info("Setting relay1: OFF")
        relay1.off()

def set_relay2(status):
    if status:
        logging.info("Setting relay2: ON")
        relay2.on()
    else:
        logging.info("Setting relay2: OFF")
        relay2.off()

def set_relay3(status):
    if status:
        logging.info("Setting relay3: ON")
        relay3.on()
    else:
        logging.info("Setting relay3: OFF")
        relay3.off()

def set_relay4(status):
    if status:
        logging.info("Setting relay4: ON")
        relay4.on()
    else:
        logging.info("Setting relay4: OFF")
        relay4.off()



def toggle_relay1():
    logging.info("toggling relay1")
    relay1.toggle()



def cocktail1():
    logging.info("Coctail1 start")
    workLoop(1, 1.2, 1, 2.5, 1, 6, 1, 2)
    logging.info("Coctail1 stop")
    
def cocktail2():
    logging.info("Coctail2 start")
    workLoop(1, 1, 1, 3, 0, 0, 1, 3)
    logging.info("Coctail2 stop")
    
def cocktail3():
    logging.info("Coctail3 start")
    workLoop(1, 5, 1, 8, 1, 0.9, 1, 4)
    logging.info("Coctail3 stop")



def workLoop(pump1, duration1, pump2, duration2, pump3, duration3, pump4, duration4):
    global isPumping
       
    red1.off()
    red2.off()
    red3.off()
    pygame.mixer.music.rewind
 
    if(isPumping is False): # in this case a new pump event can start
        isPumping = not isPumping
        
        now = time.time() #start the timer
        future1 = now + duration1
        future2 = now + duration2
        future3 = now + duration3
        future4 = now + duration4

        futureList=[future1, future2, future3, future4]
        futureMax=max(futureList)
        
        pygame.mixer.music.play()
                
        if(pump1==1):
            red1.on()
            set_relay1(True)
        if(pump2==1):
            red2.on()
            set_relay2(True)
        if(pump3==1):
            red3.on()
            set_relay3(True)
        if(pump4==1):
            red3.on()
            set_relay4(True)            

        while time.time() < futureMax+0.1:
            
            for event in pygame.event.get():        
                if event.type == SONG_END:
                    #print("the song ended!")
                    play_a_different_song()
 
            if(time.time()>=future1):
                red1.off()
                set_relay1(False)
            
            if(time.time()>=future2):
                red2.off()
                set_relay2(False)
                
            if(time.time()>=future3):
                red3.off()
                set_relay3(False)

            if(time.time()>=future4):
                set_relay4(False)                
          
    pygame.mixer.music.stop()     
    isPumping = not isPumping
    

def play_a_different_song(_list):
#    global _currently_playing_song,_list
    next_song = random.choice(_list)
#    while next_song == _currently_playing_song:
#    next_song = random.choice(_list)
#    _currently_playing_song = next_song
    pygame.mixer.music.load(next_song)
#    pygame.mixer.music.play(0)

def muenze_eingeworfen():
    global _numM
    print "buttonM pressed";
    play_a_different_song(_coinList)
    _numM += 1
    if _numM<5:
        play_a_different_song(_coinList)
        pygame.mixer.music.play(0)
        while pygame.mixer.music.get_busy():
            pygame.time.wait(100)  # ms
            print "Playing..."
        print "Finished."
        pygame.mixer.music.load('sound/muenze2.wav')
        pygame.mixer.music.play(-1)
        print _numM;
    if _numM>4:
        play_a_different_song(_coinList)
        pygame.mixer.music.play(0)
        while pygame.mixer.music.get_busy():
            pygame.time.wait(100)  # ms
        pygame.mixer.music.load("sound/cocktail.wav")
        pygame.mixer.music.play(-1)
        print _numM;

def pressed():
    print "pressed"

def released():
    print "released"
##########
if __name__=='__main__':
    try:
        logging.basicConfig(filename='/tmp/barRobot2.log',level=logging.DEBUG, format='%(asctime)s %(message)s')
        logging.info('Logging START V0.1e') 
        pygame.mixer.music.play(-1)
#        if buttonM.is_pressed == True:
#            pygame.mixer.music.stop()
#            buttonM.when_pressed =muenze_eingeworfen
#            pygame.mixer.music.load(sd_muenze2)
#            pygame.mixer.Sound.play(sd_muenze2)
#        pygame.mixer.music.queue('sound/muenze2.wav')
        buttonM.when_pressed =muenze_eingeworfen
        button1.when_released =cocktail1
        button2.when_released =cocktail2
        button3.when_released =cocktail3        
#        buttonIR.when_pressed=pressed
#        buttonIR.when_released=released
#        pir.wait_for_motion()
        pir.when_motion=pressed
        while True:
            continue
            #buttonM.when_pressed =muenze_eingeworfen        
#pause()

    except KeyboardInterrupt:
        logging.info('Applicxation KeyboardInterrupt EXIT')
        # exit the application
        sys.exit(0)
