aioseq
======

A pattern based MIDI sequencer in tracker style

The idea
--------

The resaon behind the aioseq system is to provide a fairly "programmatic" way
of composing music on a Linux based computer. The goal is to have an old-school
tracker-like and pattern based software for easy reuse of composed material
within a whole project or composition like a song or part of a song. Existing
trackers are limited, end-of-life or out-dated in many ways, and this approach
enables a very minimalistic, yet powerful, way of building musical pieces with
a programmers' precision.

The technical twists of this project is to keep everything as simple as
possible, but yet provide a full digital audio workstation for musicians with
some experience of trackers. This is achieved by making the engine of the
tracker as one process and the enabling anyone to write their own graphical
or textual front-ends towards this "server process". The latter handles
storage of data and communications with hardware with as high performance as
possible.

Object hierarchy (Draft)
------------------------

Studio
  |
  +- MIDI Instruments < - - - - - - - - - .
  |                                       |
  +- Outputs < - - - - - - - - - - - -.
  |                                   |   |
  +- Inputs  < - - - - - - - - - - -.
                                    | |   |
Project
  |                                 | |   |
  +- Samples  < - - - - - - - - - .
  |    |                          | | |   |
  |    +- Sample
  |                               | | |   |
  +- Instruments < - - - - - - - - - - -.
  |    |                          | | | | |
  |    +- Instrument
  |         |                     | | | | |
  |         +- Sample set
  |         |    |                | | | | |
  |         |    +- Samples
  |         |         |           | | | | |
  |         |         +- Sample - '
  |         |                       | | | |
  |         +- Device
  |              |                  | | | |
  |              +- Inputs
  |              |    |             | | | |
  |              |    +- Input  - - '
  |              |                    | | |
  |              +- MIDI Instrument - - - '
  |                                   | |
  +- Channels
  |    |                              | |
  |    +- Channel
  |         |                         | |
  |         +- Output - - - - - - - - '
  |         |                           |
  |         +- Instrument - - - - - - - '
  |
  +- Songs
  |    |
  |    +- Song
  |        |
  |        +- Parts
  |            |
  |            +- Part
  |                |
  |                +- Patterns
  |                     |
  |                     +- Pattern - .
  |                                  |
  +- Patterns  < - - - - - - - - - - '
       |
       +- Pattern
            |
            +- Pattern rows
                 |
                 +- Pattern row
                      |
                      +- Channel row
                           |
                           +- Notes
                           |    |
                           |    +- Note
                           |
                           +- Parameters
                                |
                                +- Parameter

Channel editor GUI idea (draft)
-------------------------------

Here are some ideas on how too configure the channels within a project that
can be used in all songs in that very project.

01 <Device> <01> Roland TD-9 <01> Bank1 <40> AiO

   Polyphony: <03> <Assigned>   Name: Note:
                             01 "Ki"  <C-1>
                             02 "Sn"  <E-1>
                             03 "HH"  <A-1>

   Params:    <01> <Assigned>   Name: Parameter:
                             01 "FT"  <40>

02 <Device> <02> C64 MSSIAH <01> Retro <01> Last Ninja Bass

   Polyphony: <01> <Unassigned> Name:
                             01 "Bass"

   Params:    <03> <Assigned>   Name: Note:
                             01 "CO"  <31>
                             02 "RE"  <32>
                             03 "PA"  <63>
                             04 "VO"  <73>

03 <Device> <03> Korg microKorg XL <01> A <32> House 2 tier

   Polyphony: <03> <Unassigned> Name:
                             01 "Bass"
                             02 "Piano1"
                             03 "Piano2"

   Params:    <02> <Unassigned> Name:
                             01 "Eff1"
                             02 "Eff2"

04 <Device> <04> DS9 <01> Bank1 <03> DigiPano1

   Polyphony: <01> <Unassigned> Name:
                             01 ""

   Params:    <01> <Unassigned> Name:
                             01 ""

05 <Samples> <01> Drum samples

   Polyphony: <03> <Assigned>   Name: Note: Sample:
                             01 "Ki"  <C-4> <01> Kick1
                             02 "Sn"  <C-4> <02> Snare1

   Params:    <01> <Unassigned> Name:
                             01 "Eff1"
                             02 "Eff2"

06 <Samples> <01> Various Retro

   Polyphony: <01> <Unassigned> Name:
                             01 ""

Pattern editor GUI idea (draft)
-------------------------------

This is just some idea scribbles describing the thinking behind why the
underlying object hierachy is defined as it is.

   01 Roland   02 C64 MSSIAH      03 Korg microKorg XL           04 DS9
   AiO         Last Ninja Bass    House 2 tier                   DigiPiano1
   Ki Sn HH FT Bass   CO RE PA VO Bass   Piano1 Piano2 Eff2 Eff1
00 7F -- 7f 40 A-2 7f 00 00 00 00 A-1 7F C-2 7F E-2 7F 0000 0000 --- 00 0000
01 -- -- -- 30 A-2 7f 00 00 00 00 --- 00 --- 00 --- 00 0000 0000 --- 00 0000
02 -- -- 7f 20 A-2 7f 00 00 00 00 A-2 00 --- 00 --- 00 0000 0000 --- 00 0000
03 -- -- -- 10 A-2 7f 00 00 00 00 --- 00 --- 00 --- 00 0000 0000 --- 00 0000
04 7f 7f 7F 40 E-2 7f 00 00 00 00 A-1 00 --- 00 A-2 7F 0000 0000 --- 00 0000
05 -- -- -- 30 E-2 7f 00 00 00 00 --- 00 --- 00 --- 00 0000 0000 --- 00 0000
06 -- -- 7f 20 E-2 7f 00 00 00 00 A-2 00 --- 00 --- 00 0000 0000 --- 00 0000
07 -- -- -- 10 E-2 7f 00 00 00 00 --- 00 --- 00 --- 00 0000 0000 --- 00 0000

... and a bit further to the right. :)

05 Drum Sampamples        06 Various Retro
                                              Mixer channel control
Ki Eff1 Eff2 Sn Eff1 Eff2                     L  R  I1 I2 S1 S2 S3 S4 P  MS
7f 0000 0000 -- 0000 0000 A-4 7f 02 0000 0000 7f 7f 00 00 00 00 00 00 00 00
-- 0000 0000 -- 0000 0000 --- -- 00 0000 0000 -- -- -- -- -- -- -- -- -- --
-- 0000 0000 -- 0000 0000 A-4 7f 02 0000 0000 -- -- -- -- -- -- -- -- -- --
-- 0000 0000 -- 0000 0000 --- -- 00 0000 0000 -- -- -- -- -- -- -- -- -- --
7f 0000 0000 7f 0000 0000 C-5 7f 02 0000 0000 -- -- -- -- -- -- -- -- -- --
-- 0000 0000 -- 0000 0000 --- -- 00 0000 0000 -- -- -- -- -- -- -- -- -- --
-- 0000 0000 -- 0000 0000 E-5 7f 02 0000 0000 -- -- -- -- -- -- -- -- -- --
-- 0000 0000 -- 0000 0000 --- -- 00 0000 0000 -- -- -- -- -- -- -- -- -- --

Here there are four channels. Channel 1 is a MIDI device with the
"percussion"-mode where each polyphonic is hard-wired to a specific
note, hence there is a posibility to name each column, just having the
velocity value as a note-on indicator. Channel 2 is a MIDI device with
a monophonic setting, but where parameters are hard-wired to a specific
value, hence there is a posibility to name each column. Channel 3 is a MIDI
device with two tiers on the keyboard - This can be done by naming the
polyphonic layout like in the example and the parameters are generic for
setting various things, not hardcoded in any way. Channel 4 is a naive mono
MIDI device with no prefered settings. Channel 5 is a sample-set with note
assigned polyphony a bit the same as Channel 1, however every polyphony
has parameters, for greater control of the samples. Channel 6 is monophonic
sample-set channel with its mixer mater control expanded.

   01 Roland                                 02 C64 MSSIAH      03 Korg micro
   AiO         Mixer channel control         Last Ninja Bass    House 2 tier
   Ki Sn HH FT L  R  I1 I2 S1 S2 S3 S4 P  MS Bass   CO RE PA VO Bass   Piano1
00 7F -- 7f 40 7f 7f 00 00 00 00 00 00 00 00 A-2 7f 00 00 00 00 A-1 7F C-2 7F
01 -- -- -- 30 -- -- -- -- -- -- -- -- -- -- A-2 7f 00 00 00 00 --- 00 --- 00
02 -- -- 7f 20 -- -- -- -- -- -- -- -- -- -- A-2 7f 00 00 00 00 A-2 00 --- 00
03 -- -- -- 10 -- -- -- -- -- -- -- -- -- -- A-2 7f 00 00 00 00 --- 00 --- 00
04 7f 7f 7F 40 -- -- -- -- -- -- -- -- -- -- E-2 7f 00 00 00 00 A-1 00 --- 00
05 -- -- -- 30 -- -- -- -- -- -- -- -- -- -- E-2 7f 00 00 00 00 --- 00 --- 00
06 -- -- 7f 20 -- -- -- -- -- -- -- -- -- -- E-2 7f 00 00 00 00 A-2 00 --- 00
07 -- -- -- 10 -- -- -- -- -- -- -- -- -- -- E-2 7f 00 00 00 00 --- 00 --- 00

The mixer channel control shall be possible to expand on every channel so that
automisation of just about everything can be handeled in one place.

Mixer GUI idea (draft) still a bit fuzzy about this bit.
--------------------------------------------------------

    01 Roland 02 C64 MS 03 Korg  04 DS9
I1  <00>      <00>      <00>     <00>
I2  <00>      <00>      <00>     <00>
S1  <00> .    <00>      <00>     <00>
S2  <00> :.   <00>      <00>     <00>
S3  <00> ::   <00>      <00>     <00>
S4  <00> ::   <00>      <00>     <00>
         ::
Pan <00>
Mon <Input>

     <M> <S>   <M> <S>   <M> <S>   <M> <S>
    -   -   - -   -   - -   -   - -   -   -
    -   -   - -   -   - -   -   - -   -   -
    -   -   - -   -   - -   -   - -   -   -
    -   -   - -   -   - -   -   - -   -   -
    -   -   - -   -   - -   -   - -   -   -
    -   -   - -   -   - -   -   - -   -   -
    -   -   - -   -   - -   -   - -   -   -
    -   -   - -   -   - -   -   - -   -   -
    -[-]-[-]- -[-]-[-]- -[-]-[-]- -[-]-[-]-
    <00> <00> <00> <00> <00> <00> <00> <00>