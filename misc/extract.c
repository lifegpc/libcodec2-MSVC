/*
  extract.c
  david Rowe Jan 2019

  Extracts sub sets of vectors from .f32 files, used for LPCNet VQ experiments.
*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#define NB_FEATURES 55 /* number of cols per row */

int main(int argc, char *argv[]) {
    FILE *fin, *fout;
    int st = 0;
    int en = 17;
    int stride = NB_FEATURES;
    float gain = 1.0;
    int frame_delay = 1;
    float pred = 0.0;
    
    static struct option long_options[] = {
        {"startcol",   required_argument, 0, 's'},
        {"endcol",     required_argument, 0, 'e'},
        {"stride",     required_argument, 0, 't'},
        {"gain",       required_argument, 0, 'g'},
        {"pred",       required_argument, 0, 'p'},
        {"delay",      required_argument, 0, 'd'},
        {0, 0, 0, 0}
    };

    int opt_index = 0;
    int c;
    
    while ((c = getopt_long (argc, argv, "s:e:t:g:p:d:", long_options, &opt_index)) != -1) {
        switch (c) {
        case 's':
            st = atoi(optarg);
            break;
        case 'e':
            en = atoi(optarg);
            break;
        case 't':
            stride = atoi(optarg);
            break;
        case 'g':
            gain = atof(optarg);
            break;
        case 'p':
            pred = atof(optarg);
            break;
        case 'd':
            frame_delay = atoi(optarg);
            break;
        default:
        helpmsg:
            fprintf(stderr, "usage: %s  -s startCol -e endCol [-t strideCol -g gain -p predCoeff -d framesDelay] input.f32 output.f32\n", argv[0]);
            exit(1);
        }
    }
    if ( (argc - optind) < 2) {
        fprintf(stderr, "Too few arguments\n");
        goto helpmsg;
    }
 
    fin = fopen(argv[optind],"rb"); assert(fin != NULL);
    fout = fopen(argv[optind+1],"wb"); assert(fout != NULL);
    printf("extracting from %d to %d inclusive (stride %d) ... gain = %f pred = %f frame_delay = %d\n",
           st, en, stride, gain, pred, frame_delay);
   
    float* features = (float*)malloc(stride * sizeof(float)), * features_prev = (float*)malloc(frame_delay * stride * sizeof(float)), * delta = (float*)malloc(stride * sizeof(float));
    int i,f;
    
    for (f = 0; f < frame_delay; f++)
        for (i = 0; i < stride; i++)
            features_prev[f * stride + i] = 0.0;

    while((fread(features, sizeof(float), stride, fin) == stride)) {
        for(i=st; i<=en; i++) {
            delta[i] = gain * (features[i] - pred * features_prev[(frame_delay - 1) * stride + i]);
        }
        fwrite(&delta[st], sizeof(float), en-st+1, fout);
        for (f = frame_delay - 1; f > 0; f--)
            for (i = 0; i < stride; i++)
                features_prev[f * stride + i] = features_prev[(f - 1) * stride + i];
        for(i=0; i<stride; i++)
            features_prev[i] = features[i];        
    }

    free(features);
    free(features_prev);
    free(delta);

    fclose(fin); fclose(fout);
    
    return 0;
}

