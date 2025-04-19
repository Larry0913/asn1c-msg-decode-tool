#include "MessageFrame.h"
#include <stdio.h>
#include <stdlib.h>

static uint16_t convert_bytes_string_to_array(uint8_t *array, uint16_t size, const char *bytes_string)
{
    const char *start = NULL;
    const char *p = NULL;
    char hex[3] = {0};
    uint16_t used_size = 0;

    start = bytes_string;
    while ((*start == 0x20) || (*start == '\n') || (*start == '\r'))
    {
        start++;
    }

    for (p = start; *p != '\0';)
    {
        memcpy(hex, p, 2);
        array[used_size++] = strtol(hex, NULL, 16);
        assert(used_size <= size);
        p += 2;
        
        while((*p == 0x20) || (*p == '\n') || (*p == '\r'))
        {
            p++;
        }
    }

    return used_size;

}

void SPAT_parse(FILE* asn_file, FILE* fp, char* asn_file_path)
{
    int num = 0;
    char phase_id[1024] = {0};
    int flag = 0;
    char data_line[1024];
    int flag1 = 0;
    char *start = data_line;
    char *data = NULL;

    char* key_array1[20] = {"msgCnt", "intersections", "intersectionId", "id", "status"};
    char* key_array2[20] = {"light", "startTime", "likelyEndTime"};
    int index1 = 0;
    int index2 = 0;

    while(fgets(data_line, 1024, asn_file) != NULL)
    {
        start = data_line;
        data = NULL;
        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
        {
            start++;
        }

        if(index1 < 5 && strstr(start, key_array1[index1]) != NULL)
        {
            index1++;
        }

        if(index2 < 3 && strstr(start, key_array2[index2]) != NULL)
        {
            index2++;
            if(index2 == 3)
            {
                index2 = 0;
            }
        }

        if (strncmp(start, "msgCnt", 6) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "msgCnt     :  %s\n", data);
        }

        int spat_list_size = 0;
        if (flag == 0)
        {
            char *temp = data_line;
            FILE *asn_temp = fopen(asn_file_path, "r");
            while(fgets(temp, 1024, asn_temp) != NULL)
            {
                if(strstr(temp, "PhaseList") != NULL)
                {
                    spat_list_size++;   
                }
            }
            fclose(asn_temp);

            flag = 1;
        }

        if(spat_list_size != 0)
        {
            fprintf(fp, "SpatListAll| %d\n", spat_list_size);
        }
        
        else if (strncmp(start, "name", 4) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "SpatName :  %s\n", data);
        }

        else if (strncmp(start, "intersectionId", 14) == 0)
        {
            if(fgets(data_line, 1024, asn_file) != NULL)
            {
                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }
                if(strncmp(start, "region", 6) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "intersection Region: %s\n", data);
                }

                if(strncmp(start, "id", 2) == 0)
                {
                    strtok(start, "d");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "intersection Id%s\n", data);
                    index1++;
                } 
                else if (fgets(data_line, 1024, asn_file) != NULL)
                {
                    start = data_line;
                    while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                    {
                        start++;
                    }
                    if(strncmp(start, "id", 2) == 0)
                    {
                        strtok(start, "d");
                        data = strtok(NULL, "\n");
                        fprintf(fp, "intersection Id%s\n", data);
                        index1++;
                    }
                }
            }
        }

        else if (strncmp(start, "status", 6) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "Status:  %s\n", data);
            fprintf(fp, "|No.|PhaseID|PhaseLight|StartTime|minEndTime|LikelyEndTime|nextDuration|\n");
        }

        else if(strncmp(start, "Phase ", 6) == 0)
        {
            if (fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }
                if(strncmp(start, "id", 2) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    memcpy(phase_id, data, sizeof(data));
                }
                else 
                {
                    printf("Information loss: id\n");
                    break;
                }
            }
            
        }

        else if (strncmp(start, "light", 5) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "%d %s %s ", num++, phase_id, data);
        }

        else if (strncmp(start, "startTime", 9) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "%s--", data);
        }

        else if (strncmp(start, "likelyEndTime", 13) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "%s--", data);
        }

        else if (strncmp(start, "nextStartTime", 13) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "%s--", data);
        }

        else if (strncmp(start, "nextDuration", 12) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "%s\n", data);
            flag = 1;
        }
    
        memset(data_line, 0, sizeof(data_line));
    }
    if(index1 < 5)
    {
        printf("Information loss: %s\n", key_array1[index1]);
        return;
    }

    if (index2 >0 && index2 < 3)
    {
        printf("Information loss: %s\n", key_array2[index2]);
    }


}

void BSM_parse(FILE* asn_file, FILE* fp, char* asn_file_path)
{
    int num = 0;
    char data_line[1024];
    char *start = data_line;
    char *data = NULL;
    int flag = 0;
    int flag1 = 0;
    int flag2 = 0;

    char* key_array[20] = {"msgCnt", "id", "secMark", "pos", "transmission", "speed", "heading", "accelSet", "brakes", "size", "vehicleClass"};
    int index = 0;
    
    while(fgets(data_line, 1024, asn_file) != NULL)
    {
        start = data_line;
        data = NULL;

        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
        {
            start++;
        }
        if(key_array[index] != NULL && strstr(start, key_array[index]) != NULL)
        {
            index++;
        }

        if (strncmp(start, "msgCnt", 6) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "msgCnt     :%s\n", data);
        }

        else if (strncmp(start, "id", 2) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "id         :%s\n", data);
        }

        else if (strncmp(start, "secMark", 7) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "secMark    :%s\n", data);
        }

        else if (strstr(start, "Position3D") != NULL && flag == 0)
        {
            flag = 1; 
            fprintf(fp, "pos: \n");
            while (fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }
                if(strncmp(start, "}", 1) == 0)
                {
                    break;
                }

                if (strncmp(start, "lat", 3) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "   lat:%s\n", data);
                }
                else if (strncmp(start, "lon", 3) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "   lon:%s\n", data);
                }
                else if (strncmp(start, "elevation", 9) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "   elevation:%s\n", data);
                }
            }
        }
        else if (strncmp(start, "speed", 5) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "speed:%s\n", data);
        }
        else if (strncmp(start, "heading", 7) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "heading:%s\n", data);
        }
        else if (strstr(start, "size") != NULL)
        {
            fprintf(fp, "size: \n");
            while (fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }
                if(strncmp(start, "}", 1) == 0)
                {
                    break;
                }

                if (strncmp(start, "width", 5) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "   width:%s\n", data);
                }
                else if (strncmp(start, "length", 6) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "   length:%s\n", data);
                }
                else if (strncmp(start, "height", 6) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "   height:%s\n", data);
                }
            }
        }
        else if (strstr(start, "vehicleClass") != NULL)
        {
            fprintf(fp, "vehicleClass: \n");
            while (fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }
                if(strncmp(start, "}", 1) == 0)
                {
                    break;
                }

                if(strncmp(start, "classification", 14) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "   classification:%s\n", data);
                }
                else if(strncmp(start, "fuelType", 8) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "   fuelType:%s\n", data);
                }
            }
        }
        else if (strncmp(start, "safetyExt", 9) == 0)
        {
            fprintf(fp, "safetyExt:\n");
            flag1 = 1; 
        }
        else if (strncmp(start, "events", 6) == 0 && flag1 == 1)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "   events:%s\n", data);
        }
        else if (strncmp(start, "pathHistory", 11) == 0 && flag1 == 1)
        {
            fprintf(fp, "   pathHistory:\n");
        }
        else if (strncmp(start, "initialPosition", 15) == 0 && flag1 == 1)
        {
            fprintf(fp, "       initialPosition:\n");
            while (fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }

                if (strncmp(start, "}", 1) == 0)
                {
                    break;
                }
                if (strncmp(start, "utcTime", 7) == 0)
                {
                    fprintf(fp, "           utcTime:\n");
                }
                else if (strncmp(start, "year", 4) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "               year:%s\n", data);
                }
                else if (strncmp(start, "month", 5) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "               month:%s\n", data);
                }
                else if (strncmp(start, "day", 3) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "               day:%s\n", data);
                }
            }

            fprintf(fp, "           pos:\n");
            while (fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }

                if (strncmp(start, "}", 1) == 0)
                {
                    break;
                }

                if (strncmp(start, "lat", 3) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "               lat:%s\n", data);
                }
                else if (strncmp(start, "lon", 3) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "               lon:%s\n", data);
                }
                else if (strncmp(start, "elevation", 9) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "               elevation:%s\n", data);
                }
            }
        }

        else if (strncmp(start, "crumbData", 9) == 0 && flag1 == 1)
        {
            fprintf(fp, "       crumbData:\n");
            flag2 = 1;
        }

        else if (strncmp(start, "PathHistoryPoint", 16) == 0 && flag2 == 1)
        {
            fprintf(fp, "           PathHistoryPoint %d:\n", num++);
        }

        else if (strncmp(start, "lon", 3) == 0 && flag2 == 1)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "                   lon:%s\n", data);
        }

        else if (strncmp(start, "lat", 3) == 0 && flag2 == 1)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "                   lat:%s\n", data);
        }

        else if (strncmp(start, "timeOffset", 10) == 0 && flag2 == 1)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "               timeOffset:%s\n",data);
        }

        else if(strncmp(start, "pathPrediction", 14) == 0 && flag1 == 1)
        {
            fprintf(fp, "   pathPrediction:\n");
            while (fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }

                if (strncmp(start, "}", 1) == 0)
                {
                    break;
                }

                if (strncmp(start, "radiusOfCurve", 13) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       radiusOfCurve:%s\n",data);
                }

                else if (strncmp(start, "confidence", 10) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       confidence:%s\n",data);
                }
            }
        }

        else if(strncmp(start, "lights", 6) == 0 && flag1 == 1)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "   lights:%s\n", data);
        }

    }
    if(index < 11)
    {
        printf("Information loss: %s\n", key_array[index]);
    }
}

void Map_parse(FILE* asn_file, FILE* fp, char* asn_file_path)
{
    char data_line[1024];
    char *start = data_line;
    char *data = NULL;
    int node_count = 0;
    int link_count = 0;
    int lane_count = 0;
    int flag = 0;
    int flag1 = 0;
    int flag2 = 0;
    int points_flag = 0;

    while(fgets(data_line, 1024, asn_file) != NULL)
    {
        start = data_line;
        data = NULL;
        flag = 0;
        flag1 = 0;

        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
        {
            start++;
        }

        if (strncmp(start, "msgCnt", 6) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "msgCnt     :%s\n", data);
            flag2 = 1;
        }

        else if (flag2 == 0)
        {
            printf("Information loss: msgCnt\n");
            break;
        }
        

        if (strncmp(start, "timeStamp", 9) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "timeStamp  :%s\n", data);
        }

        else if (strncmp(start, "Node", 4) == 0)
        {
            fprintf(fp, "Node  %d:\n", node_count++ );
            link_count = 0;
        }

        else if (strncmp(start, "id", 2) == 0)
        {
            if(fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }   
                if (strncmp(start, "region", 6) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "   id:\n       region:%s\n", data);

                    if(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }   
                        if (strncmp(start, "id", 2) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "       id:%s\n", data);
                        }
                        else 
                        {
                            printf("Information loss: Node id\n");
                            break;
                        }
                    }
                }
                else if(strncmp(start, "id", 2) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       id:%s\n", data);
                }
                else 
                {
                    printf("Information loss: Node id\n");
                    break;
                }
            }
        }

        else if (strncmp(start, "refPos", 6) == 0)
        {
            fprintf(fp, "   refPos:\n");
            if(fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }   
                if (strncmp(start, "lat", 3) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       lat:%s\n", data);
                }

                else if (strncmp(start, "lon", 3) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       lon:%s\n", data);
                }

                else 
                {
                    printf("Information loss: refPos\n");
                }
                
            }

            if(fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }   
                if (strncmp(start, "lat", 3) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       lat:%s\n", data);
                }

                else if (strncmp(start, "lon", 3) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       lon:%s\n", data);
                }

                else 
                {
                    printf("Information loss: refPos\n");
                }
            }

        }

        else if (strncmp(start, "inLinks", 7) == 0)
        {
            fprintf(fp, "   inLinks:\n");
        }

        else if (strncmp(start, "Link", 4) == 0)
        {
            fprintf(fp, "       Link %d:\n", link_count++);
            while (fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }  

                if (strncmp(start, "upstreamNodeId", 14) == 0)
                {
                    fprintf(fp, "           upstreamNodeId:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }  
                        if (strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if (strncmp(start, "region", 6) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "               region:%s\n", data);
                        }

                        else if (strncmp(start, "id", 2) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "               id:%s\n", data);
                        }
                    }
                }

                else if (strncmp(start, "speedLimits", 11) == 0)
                {
                    fprintf(fp, "           speedLimits:\n");
                    while (fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if(strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }
                        if(strncmp(start, "RegulatorySpeedLimit", 20) == 0)
                        {
                            fprintf(fp, "               RegulatorySpeedLimit:\n");
                        }

                        if (strncmp(start, "type", 4) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                   type:%s\n", data);
                        }

                        else if (strncmp(start, "speed", 5) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                   speed:%s\n", data);
                        }
                    }
                }

                else if (strncmp(start, "linkWidth", 9) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "           linkWidth:%s\n", data);
                }

                else if (strncmp(start, "points", 6) == 0)
                {
                    if(points_flag == 0)
                    {
                        fprintf(fp, "           points:\n");
                    }
                    else 
                    {
                        fprintf(fp, "                   points:\n");
                    }
                    
                    while (fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        while(strncmp(start, "}", 1) == 0)
                        {
                            if (fgets(data_line, 1024, asn_file) != NULL)
                            {
                                start = data_line;
                                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                                {
                                    start++;
                                }    

                                if(strncmp(start, "RoadPoint", 9) != 0 && strncmp(start, "}", 1) != 0)
                                {
                                    flag1 = 1;
                                    break;
                                }
                            }

                            else if (fgets(data_line, 1024, asn_file) == NULL)
                            {
                                flag1 = 1;
                                break;
                            }
                        }

                        if(strncmp(start, "RoadPoint", 9) == 0)
                        {
                            if(points_flag == 0)
                            {
                                fprintf(fp, "               RoadPoint:\n");
                            }
                            else 
                            {
                                fprintf(fp, "                       RoadPoint:\n");
                            }
                        }

                        else if (strncmp(start, "lat", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            if(points_flag == 0)
                            {
                                fprintf(fp, "                   lat:%s\n", data);
                            }
                            else 
                            {
                                fprintf(fp, "                           lat:%s\n", data);
                            }
                            
                        }

                        else if (strncmp(start, "lon", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            if(points_flag == 0)
                            {
                                fprintf(fp, "                   lon:%s\n", data);
                            }
                            else 
                            {
                                fprintf(fp, "                           lon:%s\n", data);
                            }
                        }

                        if(flag1 == 1)
                        {
                            flag1 = 0;
                            break;
                        }
                    }
                }

                if (strncmp(start, "movements", 9) == 0)
                {
                    fprintf(fp, "           movements:\n");
                }

                else if (strncmp(start, "Movement", 8) == 0)
                {
                    fprintf(fp, "               Movement:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }  
                        if (strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if(strncmp(start, "remoteIntersection", 18) == 0)
                        {
                            fprintf(fp, "                   remoteIntersection:\n");
                        }

                        else if (strncmp(start, "region", 6) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                       region:%s\n", data);
                        }

                        else if (strncmp(start, "id", 2) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                       id:%s\n", data);
                        }
                    }

                    if(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }  

                        if(strncmp(start, "phaseId", 7) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                   phaseId:%s\n",data);
                        }
                    }
                }

                else if (strncmp(start, "lanes", 5) == 0)
                {
                    fprintf(fp, "           lanes:\n");
                    if(points_flag == 0)
                    {
                        points_flag = 1;
                    }
                    else 
                    {
                        points_flag = 0;
                    }
                }

                else if (strncmp(start, "Lane", 4) == 0)
                {
                    fprintf(fp, "               Lane:\n");
                }

                else if (strncmp(start, "laneID", 6) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "                   laneID:%s\n", data);
                }

                else if (strncmp(start, "laneWidth", 9) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "                   laneWidth:%s\n", data);
                }

                else if (strncmp(start, "laneAttributes", 14) == 0)
                {
                    fprintf(fp, "                   laneAttributes:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }  
                        if (strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if (strncmp(start, "shareWith", 9) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                       shareWith:%s\n", data);
                        }

                        else if (strncmp(start, "laneType", 8) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                       laneType:%s\n", data);
                        }

                    }
                }

                else if (strncmp(start, "maneuvers", 9) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "                   maneuvers:%s\n", data);
                }

                if(strncmp(start, "Node", 4) == 0)
                {
                    fprintf(fp, "Node  %d:\n", node_count++);
                    link_count = 0;
                    if(points_flag == 0)
                    {
                        points_flag = 1;
                    }
                    else 
                    {
                        points_flag = 0;
                    }
                    break;
                }

                if(strncmp(start, "Link", 4) == 0)
                {
                    fprintf(fp, "       Link %d:\n", link_count++);
                    if(points_flag == 0)
                    {
                        points_flag = 1;
                    }
                    else 
                    {
                        points_flag = 0;
                    }
                    continue;
                }
            }
            
        }

    }
}

void RSI_parse(FILE* asn_file, FILE* fp, char* asn_file_path)
{ 
    char data_line[1024];
    char *start = data_line;
    char *data = NULL;
    int rte_flag = 0;
    int rts_flag = 0;
    int rte_count = 0;
    int rts_count = 0;
    int flag = 0;

    char* key_array[20] = {"msgCnt", "id", "refPos"};
    int index = 0;

    while(fgets(data_line, 1024, asn_file) != NULL)
    {
        start = data_line;
        data = NULL;

        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
        {
            start++;
        }

        if (key_array[index] != NULL && strstr(start, key_array[index]) != NULL)
        {
            index++;
        }

        if (strstr(start, "rtss") != NULL || strstr(start, "rtes") != NULL)
        {
            flag = 1;
        }

        if (strncmp(start, "msgCnt", 6) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "msgCnt:%s\n", data);
        }

        else if (strncmp(start, "moy", 3) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "moy:%s\n", data);
        }

        else if (strncmp(start, "id", 2) == 0 && rte_flag == 0 && rts_flag == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "id:%s\n", data);
        }

        else if (strncmp(start, "refPos", 6) == 0)
        {
            fprintf(fp, "refPos:\n");
            while(fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                data = NULL;

                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }

                if(strncmp(start, "}", 1) == 0)
                {
                    break;
                }

                if (strncmp(start, "lat", 3) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "   lat:%s\n", data);
                }

                else if (strncmp(start, "long", 4) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "   long:%s\n", data);
                }

                else if (strncmp(start, "elevation", 4) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "   elevation:%s\n", data);
                }
                
            }
            
        }

        else if (strncmp(start, "rtes", 4) == 0)
        {
            rte_flag = 1;
            fprintf(fp, "rtes:\n");
        }

        if(rte_flag == 1)
        {

            while(fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                data = NULL;

                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }

                if(strncmp(start, "RTEData", 7) == 0)
                {
                    fprintf(fp, "   RTEData %d\n", rte_count++);
                }

                else if(strncmp(start, "rteId", 5) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       rteID:%s\n", data);
                }

                else if(strncmp(start, "eventType", 9) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       eventType:%s\n", data);
                }

                else if(strncmp(start, "eventSource", 11) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       eventSource:%s\n", data);
                }

                else if(strncmp(start, "eventPos", 8) == 0)
                {
                    fprintf(fp, "       eventPos:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if(strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if(strncmp(start, "lon", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "               lon:%s\n", data);
                        }

                        else if(strncmp(start, "lat", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "               lat:%s\n", data);
                        }

                    }

                    if(fgets(data_line, 1024, asn_file) != NULL && strncmp(start, "offsetV", 7) == 0)
                    {
                        strtok(start, ":");
                        data = strtok(NULL, "\n");
                        fprintf(fp, "           offsetV:%s\n", data);
                    }
                }
                
                else if(strncmp(start, "eventRadius", 11) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       eventRadius:%s\n", data);
                }

                else if(strncmp(start, "description", 11) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       description:%s\n", data);
                }

                else if(strncmp(start, "timeDetails", 11) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       timeDetails:%s\n", data);
                }

                else if(strncmp(start, "priority", 8) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       priority:%s\n", data);
                }

                else if(strncmp(start, "referencePaths", 14) == 0)
                {
                    fprintf(fp, "       referencePaths:\n");
                }

                else if(strncmp(start, "activePath", 10) == 0)
                {
                    fprintf(fp, "           activePath:\n");
                }

                else if(strncmp(start, "PositionOffsetLLV", 17) == 0)
                {
                    fprintf(fp, "               PositionOffsetLLV:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if(strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if(strncmp(start, "lon", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                       lon:%s\n", data);
                        }

                        else if(strncmp(start, "lat", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                       lat:%s\n", data);
                        }
                    }
                    if(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if(strncmp(start, "offsetV", 7) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                   offsetV:%s\n", data);
                        }
                    }

                }

                else if(strncmp(start, "pathRadius", 10) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "           pathRadius:%s\n", data);
                }
                
                else if(strncmp(start, "referenceLinks", 14) == 0)
                {
                    fprintf(fp, "       referenceLinks:\n");
                }

                else if(strncmp(start, "ReferenceLink", 13) == 0)
                {
                    fprintf(fp, "           ReferenceLink:\n");
                }

                else if(strncmp(start, "upstreamNodeId", 12) == 0)
                {
                    fprintf(fp, "               upstreamNodeId:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if(strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if(strncmp(start, "region", 6) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                   region:%s\n", data);
                        }

                        else if(strncmp(start, "id", 2) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                   id:%s\n", data);
                        }
                    }
                }

                else if(strncmp(start, "downstreamNodeId", 12) == 0)
                {
                    fprintf(fp, "               downstreamNodeId:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if(strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if(strncmp(start, "region", 6) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                   region:%s\n", data);
                        }

                        else if(strncmp(start, "id", 2) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                   id:%s\n", data);
                        }
                    }
                }

                if (strncmp(start, "rtss", 4) == 0)
                {
                    fprintf(fp, "rtss:\n");
                    rte_flag = 0;
                    rts_flag = 1;
                    break;
                }
            }
            
        }

        if(rts_flag == 1)
        {
            while(fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                data = NULL;

                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }

                if(strncmp(start, "RTSData", 7) == 0)
                {
                    fprintf(fp, "   RTSData %d\n", rts_count++);
                }

                else if(strncmp(start, "rtsId", 5) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       rtsID:%s\n", data);
                }

                else if(strncmp(start, "signType", 8) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       signType:%s\n", data);
                }

                else if(strncmp(start, "signPos", 7) == 0)
                {
                    fprintf(fp, "       signPos:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if(strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if(strncmp(start, "lon", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           lon:%s\n", data);
                        }

                        else if(strncmp(start, "lat", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           lat:%s\n", data);
                        }

                    }
                }

                else if(strncmp(start, "description", 11) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       description:%s\n", data);
                }

                else if(strncmp(start, "referencePaths", 14) == 0)
                {
                    fprintf(fp, "       referencePaths:\n");
                }

                else if(strncmp(start, "activePath", 10) == 0)
                {
                    fprintf(fp, "           activePath:\n");
                }

                else if(strncmp(start, "PositionOffsetLLV", 17) == 0)
                {
                    fprintf(fp, "               PositionOffsetLLV:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if(strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if(strncmp(start, "lon", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                       lon:%s\n", data);
                        }

                        else if(strncmp(start, "lat", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                       lat:%s\n", data);
                        }
                    }
                    if(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if(strncmp(start, "offsetV", 7) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                   offsetV:%s\n", data);
                        }
                        
                    }

                }

                else if(strncmp(start, "pathRadius", 10) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "           pathRadius:%s\n", data);
                }
                
                else if(strncmp(start, "referenceLinks", 14) == 0)
                {
                    fprintf(fp, "       referenceLinks:\n");
                }

                else if(strncmp(start, "ReferenceLink", 13) == 0)
                {
                    fprintf(fp, "           ReferenceLink:\n");
                }

                else if(strncmp(start, "upstreamNodeId", 12) == 0)
                {
                    fprintf(fp, "               upstreamNodeId:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if(strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if(strncmp(start, "region", 6) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                   region:%s\n", data);
                        }

                        else if(strncmp(start, "id", 2) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                   id:%s\n", data);
                        }
                    }
                }

                else if(strncmp(start, "downstreamNodeId", 12) == 0)
                {
                    fprintf(fp, "               downstreamNodeId:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if(strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if(strncmp(start, "region", 6) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                   region:%s\n", data);
                        }

                        else if(strncmp(start, "id", 2) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "                   id:%s\n", data);
                        }
                    }
                }
            }
        }
    }

    if (index < 3)
    {
        printf("Information loss: %s\n", key_array[index]);
    }
    if (flag == 0)
    {
        printf("Information loss: rtes and rtss\n");
    }
}

void RSM_parse(FILE* asn_file, FILE* fp, char* asn_file_path)
{
    char data_line[1024];
    char *start = data_line;
    char *data = NULL;
    int participant_count = 0;
    int current_num = 0;

    char* key_array[20] = {"msgCnt", "id", "refPos", "participants"};
    char* key_array1[20] = {"ptcType", "ptcId", "source", "secMark", "pos", "posConfidence", "speed", "heading"};
    int index = 0;
    int index1 = 0;

    while(fgets(data_line, 1024, asn_file) != NULL)
    {

        start = data_line;
        data = NULL;

        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
        {
            start++;
        }

        if (key_array[index] != NULL && strstr(start, key_array[index]) != NULL)
        {
            index++;
        }

        if (strncmp(start, "msgCnt", 6) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "msgCnt:%s\n", data);
        }

        else if (strncmp(start, "id", 2) == 0)
        {
            strtok(start, ":");
            data = strtok(NULL, "\n");
            fprintf(fp, "id:%s\n", data);
        }

        else if (strncmp(start, "refPos", 6) == 0)
        {
            fprintf(fp, "refPos:\n");
            while(fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                data = NULL;

                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }

                if(strncmp(start, "}", 1) == 0)
                {
                    break;
                }

                if (strncmp(start, "lat", 3) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "   lat:%s\n", data);
                }

                else if (strncmp(start, "long", 4) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "   long:%s\n", data);
                }

                else if (strncmp(start, "elevation", 4) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "   elevation:%s\n", data);
                }
            }
        }

        else if (strncmp(start, "participants", 12) == 0)
        {
            fprintf(fp, "participants:\n");
        }

        else if (strncmp(start, "ParticipantData", 15) == 0)
        {

            fprintf(fp, "   ParticipantData %d\n", participant_count++);
            while(fgets(data_line, 1024, asn_file) != NULL)
            {
                start = data_line;
                data = NULL;

                while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                {
                    start++;
                }

                if (key_array1[index1] != NULL && strstr(start, key_array1[index1]) != NULL)
                {
                    index1++;
                }

                if (strncmp(start, "ptcType", 7) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       ptcType:%s\n", data);
                }

                else if (strncmp(start, "ptcId", 5) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       ptcId:%s\n", data);
                }
                
                else if (strncmp(start, "source", 6) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       source:%s\n", data);
                }
                
                else if (strncmp(start, "secMark", 6) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       secMark:%s\n", data);
                }
                
                else if (strncmp(start, "pos:", 4) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       pos:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if (strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if (strncmp(start, "lon", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "               lon:%s\n", data);
                        }

                        else if (strncmp(start, "lat", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "               lat:%s\n", data);
                        }
                    }

                    if (fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if(strncmp(start, "offsetV", 7) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           offsetV:%s\n", data);
                        }
                    }

                }
                
                else if (strncmp(start, "posConfidence", 13) == 0)
                {
                    fprintf(fp, "       posConfidence:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if (strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if (strncmp(start, "pos", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           pos:%s\n", data);
                        }

                        else if (strncmp(start, "elevation", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           elevation:%s\n", data);
                        }
                    }
                }

                else if (strncmp(start, "transmission", 12) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       transmission:%s\n", data);
                }

                else if (strncmp(start, "speed", 5) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       speed:%s\n", data);
                }

                else if (strncmp(start, "heading", 7) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       heading:%s\n", data);
                }

                else if (strncmp(start, "motionCfd", 9) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       motionCfd:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if (strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if (strncmp(start, "speedCfd", 8) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           speedCfd:%s\n", data);
                        }

                        else if (strncmp(start, "headingCfd", 10) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           headingCfd:%s\n", data);
                        }

                        else if (strncmp(start, "steerCfd", 8) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           steerCfd:%s\n", data);
                        }
                    }
                }

                else if (strncmp(start, "accelSet", 8) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       accelSet:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if (strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if (strncmp(start, "long", 4) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           long:%s\n", data);
                        }

                        else if (strncmp(start, "lat", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           lat:%s\n", data);
                        
                        }
                        else if (strncmp(start, "vert", 4) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           vert:%s\n", data);
                        }

                        else if (strncmp(start, "yaw", 3) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           yaw:%s\n", data);
                        }
                    }
                }

                else if (strncmp(start, "size", 4) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       size:\n");
                    while(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if (strncmp(start, "}", 1) == 0)
                        {
                            break;
                        }

                        if (strncmp(start, "width", 5) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           width:%s\n", data);
                        }

                        else if (strncmp(start, "length", 6) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           length:%s\n", data);
                        }

                        else if (strncmp(start, "height", 6) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           height:%s\n", data);
                        }
                    }
                }

                else if (strncmp(start, "vehicleClass", 12) == 0)
                {
                    strtok(start, ":");
                    data = strtok(NULL, "\n");
                    fprintf(fp, "       vehicleClass:\n");
                    if(fgets(data_line, 1024, asn_file) != NULL)
                    {
                        start = data_line;
                        data = NULL;

                        while ((*start == '\n') || (*start == '\t') || (*start == ' '))
                        {
                            start++;
                        }

                        if (strncmp(start, "classification", 14) == 0)
                        {
                            strtok(start, ":");
                            data = strtok(NULL, "\n");
                            fprintf(fp, "           classification:%s\n", data);
                        }

                    }
                }

                else if (strncmp(start, "ParticipantData", 15) == 0)
                {
                    fprintf(fp, "   ParticipantData %d\n", participant_count++);
                }

                if (index1 < 8 && participant_count > current_num + 1)
                {
                    printf("participant %d lose information: %s\n", current_num, key_array1[index1]);
                    exit(0);
                }
                else if (index1 == 8)
                {
                    current_num++;
                    index1 = 0;
                }
            }
            if(index1 < 8 && index1 != 0)
            {
                printf("participant %d lose information: %s\n", current_num, key_array1[index1]);
            }
        }
    }

    if (index < 4)
    {
        printf("Information loss: %s\n", key_array[index]);
    }
}

char *uPERDecode()
{
	// UPER 解码
    char *file_name = "data.txt";
    FILE *fp1 = fopen(file_name, "r");
    
    char *contents;
    int fileSize = 0;
    if(fp1 != NULL)
    {
        fseek(fp1, 0L, SEEK_END);
        fileSize = ftell(fp1);
        fseek(fp1, 0L, SEEK_SET);

        contents = (char *)malloc(fileSize + 1);

        size_t size_1 = fread(contents, 1, fileSize, fp1);
        //printf("size_1 is :%d\n", size_1);

        contents[size_1] = 0;
        //printf("filesize is: %d\n", fileSize);
    }
    else
    {
        printf("fail to open file\n");
    }

    asn_dec_rval_t rval;
    //MessageFrame_t *msg = (MessageFrame_t *)malloc(sizeof(MessageFrame_t));
    MessageFrame_t *msg = NULL;
    uint8_t buffer[4096] = {0}; 
    uint16_t size = convert_bytes_string_to_array(buffer, sizeof(buffer), contents);

    // for (int k = 0; k < size; k++)
    // {
	//     printf("%d: %d--0x%02x \n", k, size, buffer[k]);
    // }

	rval = uper_decode_complete(NULL, &asn_DEF_MessageFrame, (void **) &msg, buffer, size);
	if (RC_OK == rval.code ) {
		printf("\n ----- decode ASN success-----\n");
	} 
    else 
    {
		printf("\n ----- decode ASN failed ------\n");
		exit(1);
	}

    char *result_file = "result.txt";
    FILE *fp2 = fopen(result_file,"w");
    if(fp2 == NULL)
    { 
        printf("fail to create file\n");
    }
 
	// printf the msg
	//xer_fprint(fp2, &asn_DEF_MessageFrame, msg);
    asn_fprint(fp2, &asn_DEF_MessageFrame, msg);

	// 执行其他业务

    fclose(fp1);
    fclose(fp2);
    free(contents);

    return result_file;
    
}

void asn_file_parse(char *asn_file_path)
{
    char data_line[1024];
    FILE *asn_file = fopen(asn_file_path, "r");
    FILE *fp = fopen("parse_file.txt", "w");
    char *msg_frame = NULL;
    

    if(fgets(data_line, 1024, asn_file) != NULL)
    {
        if (strncmp(data_line, "BasicSafetyMessage", 18) == 0)
        {
            msg_frame = "BSM";
        }
        else if (strncmp(data_line, "MapData", 7) == 0)
        {
            msg_frame = "Map";
        }
        else if (strncmp(data_line, "RoadsideSafetyMessage", 21) == 0)
        {
            msg_frame = "RSM";
        }
        else if (strncmp(data_line, "SPAT", 4) == 0)
        {
            msg_frame = "SPAT";
        }
        else if (strncmp(data_line, "RoadSideInformation", 19) == 0)
        {
            msg_frame = "RSI";
        }
        memset(data_line, 0, sizeof(data_line));
        fprintf(fp, "消息类型    : %s\n", msg_frame);
        fprintf(fp, "===============================\n");
    }

    if (msg_frame == "BSM")
    {
        BSM_parse(asn_file, fp, asn_file_path);
    }
    else if (msg_frame == "Map")
    {
        Map_parse(asn_file, fp, asn_file_path);
    }
    else if (msg_frame == "RSM")
    {
        RSM_parse(asn_file, fp, asn_file_path);
    }
    else if (msg_frame == "SPAT")
    {
        SPAT_parse(asn_file, fp, asn_file_path);
    }
    else if (msg_frame == "RSI")
    {
        RSI_parse(asn_file, fp, asn_file_path);
    }
    
    fclose(asn_file);
    fclose(fp);
    
}

int main()
{
    //uPEREncode();

    //uPERDecode();

    asn_file_parse(uPERDecode());
    //asn_file_parse("result.txt");

    
 
    return 0;
}
