'''
there is a switch that controlles the two modi-maintenace and mixing drinks

if choosen maintenance
pumps suck as long as button is pressed

if choosen mixing drinks
program is basically an state machine and starts in idle state
sensor detects if someone is nearby and the program asks for money (twice). there is a time frame for interactions.
if time runs out, program goes back to idle. 
if money was given during the time period, the programm checks, if a glas exist. if so, a cocktail can be selected and is made.
after making the cocktail, script checks if glas is removed and if so, goes back to idle
'''
####
from gpiozero import DistanceSensor, Button, OutputDevice
from time import sleep
from signal import pause
import random
import time
import pygame
import wave
import logging

global actualState

button1=Button(16) #grün
button2=Button(20) # gelb
button3=Button(21) # orange
#pressureSensor=Button(12) # lila  

RELAY1_PIN = 18 #gelb
RELAY2_PIN = 23 #blau
RELAY3_PIN = 24 #lila
RELAY4_PIN = 25 #grau
relay1 = OutputDevice(RELAY1_PIN, active_high=False, initial_value=False)
relay2 = OutputDevice(RELAY2_PIN, active_high=False, initial_value=False)
relay3 = OutputDevice(RELAY3_PIN, active_high=False, initial_value=False)
relay4 = OutputDevice(RELAY4_PIN, active_high=False, initial_value=False)


buttonCoin = Button(2) #Münzeinwurf
pressureSensor=Button(12) # lila 
buttonBarRobotOrMaintenance=Button(26)#switch for "cleaning/sucking in" OR "cocktail mixing"  

sensor = DistanceSensor(4, 17, max_distance=1, threshold_distance=0.8)

_soundListCoin = ['/home/pi/python/DistanceTEST/1.wav', '/home/pi/python/DistanceTEST/2.wav']
_soundListSeven=['/home/pi/python/DistanceTEST/sevenofnine/activity_uno_use.wav','/home/pi/python/DistanceTEST/sevenofnine/insertacoin.wav', '/home/pi/python/DistanceTEST/sevenofnine/insertanothercoin.wav', '/home/pi/python/DistanceTEST/sevenofnine/cocktail.wav','/home/pi/python/DistanceTEST/sevenofnine/fun.wav','/home/pi/python/DistanceTEST/sevenofnine/activity_unproductive.wav', '/home/pi/python/DistanceTEST/sevenofnine/pleasant_aftern_I_w_like_to_talk.wav']
_soundListMix1 = ['/home/pi/python/barRobot-service/1.wav', '/home/pi/python/barRobot-service/2.wav', '/home/pi/python/barRobot-service/3.wav','/home/pi/python/barRobot-service/4.wav']
_currently_playing_song = None


pygame.init()

SONG_END = pygame.USEREVENT + 1
pygame.mixer.music.set_endevent(SONG_END)


def button1maintenance():
    if button1.is_pressed:                
        return True
    else:
        return False
    
def button2maintenance():
    if button2.is_pressed:
        return True
    else:
        return False
    
def button3maintenance():
    if button3.is_pressed:                
        return True
    else:
        return False   

def glasThere():
    if pressureSensor.is_pressed:
        return True
    else:
        return False

def set_relay1(status):
    if status:
        #logging.info("Setting relay1: ON")
        relay1.on()
    else:
        #logging.info("Setting relay1: OFF")
        relay1.off()

def set_relay2(status):
    if status:
        relay2.on()
    else:
        relay2.off()

def set_relay3(status):
    if status:
        relay3.on()
    else:
        #logging.info("Setting relay3: OFF")
        relay3.off()

def set_relay4(status):
    if status:
        #logging.info("Setting relay4: ON")
        relay4.on()
    else:
        #logging.info("Setting relay4: OFF")
        relay4.off()


def play_a_different_song(theList, theSong):
    pygame.mixer.music.rewind
    global _songListDialog, _songListCoins
    next_song = theList[theSong]
    pygame.mixer.music.load(next_song)
    pygame.mixer.music.play(0,0.0)


def play_a_different_song1():
    global _currently_playing_song, _soundListMix1
    next_song = random.choice(_soundListMix1)
    while next_song == _currently_playing_song:
        next_song = random.choice(_soundListMix1)
    _currently_playing_song = next_song
    pygame.mixer.music.load(next_song)
    pygame.mixer.music.play()

def soundLoopWithPauses(pauseLength,soundlist,whichSound):
    global timerSoundIsSet,timeWindowForPause, _songListWithLength 
    if timerSoundIsSet==False:
        now = time.time() #start the timer
        timeWindowForPause=now+pauseLength
        timerSoundIsSet=True
    
        soundToPlay=soundlist[whichSound]
        pygame.mixer.music.load(soundToPlay)
        pygame.mixer.music.play(0,0.0)
                          
    if timerSoundIsSet==True and time.time() >= timeWindowForPause:
        timerSoundIsSet=False
        
def maintenanceMode():
    if buttonBarRobotOrMaintenance.is_pressed:                
        return True
    else:
        return False
        
      
def playSoundAndWatchButton(timeFrame, nextZustand,soundLoop, soundList, wichSound):
    global timerIsSet,timeWindowForInput, actualState, CocktailMode
    
    if maintenanceMode()==False:

        if timerIsSet==False:      
            now = time.time() #start the timer
            timeWindowForInput=now+timeFrame        
            timerIsSet=True

                          
        if timerIsSet==True and time.time() <= timeWindowForInput:
            if soundLoop==True:
                soundLoopWithPauses(3, soundList, wichSound)
            if actualState == stateList[1] or actualState == stateList[2]:
                if buttonCoin.is_pressed:
                    play_a_different_song(_soundListCoin, 0)
                    sleep(5)
                    actualState=stateList[nextZustand]                       
                    timerIsSet=False
                
     #####pumping
            if actualState == stateList[3]:     
                if button1.is_pressed:
                    play_a_different_song(_soundListSeven, 4)
                    timerIsSet=False
                    sleep(5)
                    # wodka orange
                    # osaft 15sec 
                    # wodka 20sec    
                    workLoop(0, 15, 20, 0)
                
                elif button2.is_pressed:
                    play_a_different_song(_soundListSeven, 4)
                    timerIsSet=False
                    sleep(5)
                    # gruene wiese ca 130ml
                    # curacao  20sec
                    # osaft 15 sec
                    workLoop(20, 15, 0, 0)
                
                elif button3.is_pressed:
                    play_a_different_song(_soundListSeven, 4)
                    timerIsSet=False
                    sleep(5)
                    # osaft 15sec
                    # cranberry 20sec
                    workLoop(0, 15, 0, 20)
           
    
                                                                    
        if timerIsSet==True and time.time() > timeWindowForInput:
            #if someone stands bevore sensor the only thing he should be hearing is "put in the coin" and not that the event is abortet
            #in this case the stat switch between stateList[0]and stateList[1]
            if actualState!=stateList[1]: 
                play_a_different_song(_soundListSeven,5)
                sleep(5)   
            timerIsSet=False
            actualState=stateList[0] # prokrastinieren - da passiert nix mehr-> leg dich wieder hin
            
     
def workLoop(duration1, duration2, duration3, duration4):
    global isPumping, actualState
    
    actualState=stateList[4]
    
    if(isPumping is False): # in this case a new pump event can start
        isPumping = not isPumping
        
        now = time.time() 
        future1 = now + duration1
        future2 = now + duration2
        future3 = now + duration3
        future4 = now + duration4

        futureList=[future1, future2, future3, future4]
        futureMax=max(futureList)
    
                
        if(duration1>0):
            set_relay1(True)
        if(duration2>0):
            set_relay2(True)
        if(duration3>0):
            set_relay3(True)
        if(duration4>0):
            set_relay4(True)            

        while time.time() < futureMax+0.1:
            print("ich mixe")
            for event in pygame.event.get():        
                if event.type == SONG_END:
                    play_a_different_song1()
 
            if(time.time()>=future1):
                set_relay1(False)
            
            if(time.time()>=future2):
                set_relay2(False)
                
            if(time.time()>=future3):
                set_relay3(False)

            if(time.time()>=future4):
                set_relay4(False)                
          
    pygame.mixer.music.stop()     
    isPumping = not isPumping
    play_a_different_song(_soundListSeven, 6)

    #sleep(15)
    actualState = stateList[5]
               
    
    
########
if __name__=='__main__':    
    logging.basicConfig(filename='barRobot.log',level=logging.DEBUG, format='%(asctime)s %(message)s')
    logging.info('Logging START V0.')   

    timerIsSet=False
    timerSoundIsSet=False
    
    stateList=["prokrastinieren","kaltaquise","auftragsklaerung","auftragserfuellung", "produktion","abholung","maintenance"]#"auftragsklaerung",
    actualState=stateList[0] #prokrastinieren

    isPumping=False

    
    while True:
        try:
            
            if maintenanceMode()==True:
                actualState=stateList[6] #maintenance
              
            if actualState=="maintenance" and maintenanceMode()==True:

                if button1maintenance()==True:
                    set_relay1(True)
                if button1maintenance()==False:
                    set_relay1(False)

                if button2maintenance()==True:
                    set_relay2(True)
                if button2maintenance()==False:
                    set_relay2(False)
 
                if button3maintenance()==True:
                    set_relay3(True)
                if button3maintenance()==False:
                    set_relay3(False)
 
 
            elif actualState=="maintenance" and maintenanceMode()==False:
                actualState=stateList[0]
             
            
            if actualState=="prokrastinieren":
                #maybe an idle state would be nice globoli loocking around in random time intervalles or something
                print(actualState)
                if (sensor.distance * 100) < 20:
                    play_a_different_song(_soundListSeven, 1)                                                 
                    actualState=stateList[1]  #kaltaquise
                                         
            if actualState=="kaltaquise":
                print(actualState)
                playSoundAndWatchButton(5, 2, False, None, None) # auftragsklaerung
                
            elif actualState=="auftragsklaerung":
                print(actualState)
                playSoundAndWatchButton(15, 3, True, _soundListSeven,2) # auftragserfuellung
               
            if actualState=="auftragserfuellung": 
                if glasThere()==True:
                    print(actualState)
                    playSoundAndWatchButton(25, 4, True, _soundListSeven, 3) # prokrastinieren
                else:
                    #alert that no glas is detected
                    play_a_different_song(_soundListMix1, 1)  
                                    
            if actualState=="produktion":
                workLoop()
                #fertig=workLoop() # das returnt NIX!!!!!
                
                #if fertig==False:
                    #actualState=5
                    
            if actualState=="abholung":
                print(actualState)
                if glasThere()==False:
                     actualState=stateList[0]

             
        except KeyboardInterrupt:
            logging.info('Applicxation KeyboardInterrupt EXIT')           
            sys.exit(0)

