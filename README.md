# Projekt
Syftet med uppgiften var att bevisa sina lärda kunskaper från kursen
samt lära sig att använda sig av externa bibliotek. Jag valde SFML bibioteket
och beslutade mig för att skapa en Synthesizer som genererar ljud i realtid.
Synthen innehåller även några funktioner för att manipulera ljudet. 

Projektet blev lite för ambitiöst på ljudfronten, så den visuella fronten
är väldigt försummad. Medföljande bilden input_explanation finns som tröst i att försöka
hjälpa användaren att förstå vilka tangenter som används och vad som gör vad.


## Kort beskrivning av synthprogrammet 
Programmet kan generera 4 olika ljudvågor. Antingen separata eller kombinerade.
Dessa vågor är:

- Fyrkantsvåg
- Triangelvåg
- Sinusvåg
- Sågtandsvåg

Som instruktionsbilden förklarar så har alla vågor 2 dedikerade knappar på tangentbordet(1-8) för 
att minska/öka amplituden. För ökning används den vänstra sidan ,
och för minskning används vänster sida.   

- Attack (Manipulerar hur lång tid det tar för ljudet att uppnå maxvolym)
- Decay (Manipulerar hur lång tid det tar för ljudet att uppstå)
- Oktav (Kompenserar över tangentbordets begränsade yta genom att förskjuta
input_t 12 noter fram eller bakåt)
- PWM (Ändrar fyrkantsvågens bredd)

## Bygginstruktioner hämta från git
Tar lite längre tid att bygga ihop än om man laddar ner mingw och sfml lokalt. Men är mycket smidigare(Kräver dock att cmake är installerat). Ersätt texten i CMakeList.tx med texten nedan om du vill slippa det lokala trasslandet:

cmake_minimum_required(VERSION 3.29)
project(Synth_project)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp
        input_base.h
        MyStream.cpp
        MyStream.h
        note_data.h
        sound_modifier_data.h
        MyStream.cpp
        main.cpp)

include(FetchContent)
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL SYSTEM
)

FetchContent_MakeAvailable(SFML)

target_link_libraries(main SFML::System SFML::Window SFML::Graphics SFML::Network SFML::Audio)

Byggkommandon:   
cmake -B build -G "MinGW Makefiles"  
cmake --build build

Det körbara programmet hittas sedan som "main.exe" i build-mappen


## Bygginstruktioner lokalt
För att försäkra att programmet byggs korrekt bör följande MINGW toolchain och SFML version användas laddas ner och läggas in i projektmappen:

https://www.sfml-dev.org/download/sfml/3.0.0/
WinLibs UCRT 14.2.0 (64-bit)
GCC 14.2.0 MinGW (SEH) (UCRT)

packa upp filerna och lägg in SFML-3.0.0 i SFML mappen och mingw64 i MinGW mappen, likt:

MinGW  
└───mingw64

SFML  
└───SFML-3.0.0
    
I hänsyn till potentiella lokala MINGW-kataloger bör också environment-path
temporärt sättas till projektkatalogens MinGW\mingw64\bin-mapp

**Exempel på att hur man sätter en temporär environment-path
och bygger programmet från Powershell:**  

Sätt CD till projectkatalogen. Exempelvis:  
cd C:\Users\Your_username\Downloads\Synth_Project

Byggkommandon:

$env:PATH = "$pwd\MinGW\mingw64\bin;"  
cmake -B build -G "MinGW Makefiles"  
cmake --build build

Det körbara programmet hittas sedan som "main.exe" i build-mappen


## Förbättringsförslag
Programvolymen är hårdkodat med stream.setVolume(100) tyvärr.
Dock uppfyller minskandet av vågornas amplitud i stort sett
samma syfte som att minska volymen. Man hade eventuellt kunnat sätta en inputlistener 
som ökar eller minskar ett värde och kallar på setVolume() varje gång värdet ändras?

Det är som sagt inget visuellt i programmet mer än att en programruta öppnas. 
Det hade varit mer pedagogiskt om programmet hade en visuell layout av ett
pianotangentbord som visar var input_t sker. Någon sorts visuell feedback på alla
modifierares tillstånd hade också varit fint. Exempelvis ljudvågornas amplitud och
om "sustainOn" är aktiverad.  

Ett roligt tillägg hade varit om man visualiserade ljudvågens form under uppspelning. 
Det borde egentligen bara vara att man ta ut alla värden på samplingen som består
av y-värden och rita upp dom från vänster till höger.

Hade varit fint om jag hade implementerat en variabel i ljudvågsobjekten som hade kunnat 
ändra frekvensen något för att få lite mer spännande harmonics. 

I efterhand känns tangent-input_t rätt begränsat eftersom programmet i nuläget förlitar sig 
på att det finns en numpad för att kunna modifera vissa funktioner. Samt 
så har många tangentbord en begränsad mängd i antaler knappar och kombinationer man kan ha intryckta 
samtidigt, vilket kan förhindra exemeplvis modifiering av vågornas amplitud under spelning
och göra vissa ackord omöjliga att spela. Problemet hade kunnat lösas
om man fick programmet att fungera tillsammans med externa midi-tangentbord men det
känns väldigt överkurs. En annan lösning hade kunnat vara att användaren kan modifiera
vilka tangenter som gör vad under körning.