Sub shellSort( ar( ) As String )													/* Assume an input starting at element 0 with an upper bound of 10*/
   Dim Lower As Integer                                   /*                                                                */
   Dim Upper As Integer                                   /*                                                                */
   Dim botMax As Integer                                  /*                                                                */
   Dim i As Integer                                       /*                                                                */
   Dim k As Integer                                       /*                                                                */
   Dim h As Integer                                       /*                                                                */
   Dim v As String                                        /*                                                                */
                                                          /*                                                                */
   Lower = Lbound( ar( ) )																/* lower = 0                                                      */
   Upper = Ubound( ar( ) )																/* upper = 10                                                     */
                                                          /*                                                                */
   h = 1                                                  /*                                                                */
   Do                                                     /*                                                                */
      h = (3*h) + 1																				/* h = 4                                                          */
      Loop Until h > Upper-Lower+1												/* until h > 11                                                   */
         Do                                               /*                                                                */
            h = h \ 3																			/* h = 1                                                          */
            botMax = Lower + h - 1												/* botmax = 0                                                     */
            For i = botMax + 1 To Upper										/* i = 1 to 10                                                    */
               v = ar( i )																/* set v, a temp string hold area to array element indexed by 1   */
               k = i                                      /* set k = 1                                                      */
               While ar( k - h ) > v                      /* while array value indexed by 1 -1 (0) > value of v             */
                  ar( k ) = ar( k - h )                   /* array(1) = array(0)                                            */
                  k = k - h                               /* k = 0                                                          */
                  If (k <= botMax) Then Goto wOut         /* if 0 <= 0 then jump out                                        */
               Wend                                       /*                                                                */
wOut:                                                     /*                                                                */
   If (k <> i) Then ar(k) = v                             /* if 0 <> 1 then set array(0) to v                               */
      Next                                                /*                                                                */
      Loop Until h = 1                                    /*                                                                */
End Sub                                                   /*                                                                */