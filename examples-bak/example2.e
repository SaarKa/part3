COPYI I1 0
COPYI I2 0
COPYI I3 0
COPYI I4 0
COPYI I5 0
COPYI I6 0
COPYI I7 0
JLINK 112
HALT
ADD2I I2 I2 4
LOADI I3 I1 -12
COPYI I4 0
SEQUI I5 I3 I4
BREQZ I5 19
UJUMP 16
COPYI I6 1
STORI I6 I1 -4
RETRN
LOADI I7 I1 -12
COPYI I8 1
SEQUI I9 I7 I8
BREQZ I9 27
UJUMP 24
LOADI I10 I1 -8
STORI I10 I1 -4
RETRN
ADD2I I11 I1 0
LOADI I12 I1 -8
LOADI I13 I1 -12
COPYI I14 1
SUBTI I15 I13 I14
CITOF F2 I2
STORI I0 I2 0
STORF F0 F2 0
STORI I1 I2 4
STORF F1 F2 4
STORI I2 I2 8
STORF F2 F2 8
STORI I3 I2 12
STORF F3 F2 12
STORI I4 I2 16
STORF F4 F2 16
STORI I5 I2 20
STORF F5 F2 20
STORI I6 I2 24
STORF F6 F2 24
STORI I7 I2 28
STORF F7 F2 28
STORI I8 I2 32
STORF F8 F2 32
STORI I9 I2 36
STORF F9 F2 36
STORI I10 I2 40
STORF F10 F2 40
STORI I11 I2 44
STORF F11 F2 44
STORI I12 I2 48
STORF F12 F2 48
STORI I13 I2 52
STORF F13 F2 52
STORI I14 I2 56
STORF F14 F2 56
STORI I15 I2 60
STORF F15 F2 60
ADD2I I2 I2 76
COPYI I1 I2
CITOF F1 I1
STORI I12 I1 -8
STORI I15 I1 -12
JLINK 10
COPYI I2 I1
LOADI I16 I1 -4
SUBTI I2 I2 76
CITOF F2 I2
LOADF F0 F2 0
LOADI I0 I2 0
LOADF F1 F2 4
LOADI I1 I2 4
LOADF F3 F2 12
LOADI I3 I2 12
LOADF F4 F2 16
LOADI I4 I2 16
LOADF F5 F2 20
LOADI I5 I2 20
LOADF F6 F2 24
LOADI I6 I2 24
LOADF F7 F2 28
LOADI I7 I2 28
LOADF F8 F2 32
LOADI I8 I2 32
LOADF F9 F2 36
LOADI I9 I2 36
LOADF F10 F2 40
LOADI I10 I2 40
LOADF F11 F2 44
LOADI I11 I2 44
LOADF F12 F2 48
LOADI I12 I2 48
LOADF F13 F2 52
LOADI I13 I2 52
LOADF F14 F2 56
LOADI I14 I2 56
LOADF F15 F2 60
LOADI I15 I2 60
STORI I16 I11 0
LOADI I17 I1 0
LOADI I18 I1 -8
MULTI I19 I17 I18
STORI I19 I1 -4
RETRN
RETRN
ADD2I I2 I2 4
ADD2I I2 I2 4
ADD2I I2 I2 4
ADD2I I3 I1 0
READI I4
STORI I4 I3 0
ADD2I I5 I1 8
READI I6
STORI I6 I5 0
ADD2I I7 I1 4
LOADI I8 I1 0
LOADI I9 I1 8
CITOF F2 I2
STORI I0 I2 0
STORF F0 F2 0
STORI I1 I2 4
STORF F1 F2 4
STORI I2 I2 8
STORF F2 F2 8
STORI I3 I2 12
STORF F3 F2 12
STORI I4 I2 16
STORF F4 F2 16
STORI I5 I2 20
STORF F5 F2 20
STORI I6 I2 24
STORF F6 F2 24
STORI I7 I2 28
STORF F7 F2 28
STORI I8 I2 32
STORF F8 F2 32
STORI I9 I2 36
STORF F9 F2 36
ADD2I I2 I2 52
COPYI I1 I2
CITOF F1 I1
STORI I8 I1 -8
STORI I9 I1 -12
JLINK 10
COPYI I2 I1
LOADI I10 I1 -4
SUBTI I2 I2 52
CITOF F2 I2
LOADF F0 F2 0
LOADI I0 I2 0
LOADF F1 F2 4
LOADI I1 I2 4
LOADF F3 F2 12
LOADI I3 I2 12
LOADF F4 F2 16
LOADI I4 I2 16
LOADF F5 F2 20
LOADI I5 I2 20
LOADF F6 F2 24
LOADI I6 I2 24
LOADF F7 F2 28
LOADI I7 I2 28
LOADF F8 F2 32
LOADI I8 I2 32
LOADF F9 F2 36
LOADI I9 I2 36
STORI I10 I7 0
LOADI I11 I1 4
PRNTI I11
RETRN
