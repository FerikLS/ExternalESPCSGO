#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#include <Windows.h>

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <iostream>

#include "NavigationFunc.h"
#include "Memory.h"
#include "Offsets.h"
#include "DataVectors.h"
#include "Helper.h"
#include "DrawingFunctions.h"
 

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";

    //Get Primary Monitor and Monitor size
    GLFWmonitor* Monitor = glfwGetPrimaryMonitor();
    if (!Monitor)
    {
        return 0;
    }
    int height = glfwGetVideoMode(Monitor)->height;
    int width = glfwGetVideoMode(Monitor)->width;

    //Making Window with floating, maximized, transparent framebuffer and without resizable
    glfwWindowHint(GLFW_FLOATING, true);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);


    // Create window with graphics context
    GLFWwindow* Window = glfwCreateWindow(width, height, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (Window == NULL)
        return 1;

    ShowWindow(GetConsoleWindow(), SW_HIDE);

    //Delete top of window
    glfwSetWindowAttrib(Window, GLFW_DECORATED, false);

    glfwMakeContextCurrent(Window);
    glfwSwapInterval(1); // Enable vsync


    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed To Initialize OpenGL Loader!\n");
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    

    Memory Mem;

    //Get Process
    uintptr_t ProccessID = Mem.GetProcessID(L"csgo.exe");
    //Get Base Address of game
    uintptr_t ClientBaseAddress = Mem.GetModuleBaseAddress(ProccessID, L"client.dll");

    HANDLE HandleOfProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProccessID);

    if (ClientBaseAddress)
    {
        std::cout << "Client found: " << std::hex << ClientBaseAddress;
    }
    else
    {
        std::cout << "fuck you bro";
    }

    uintptr_t AddressLocalPlayer;
    do
    {
        AddressLocalPlayer = Mem.ReadMemory<uintptr_t>(HandleOfProcess, ClientBaseAddress + offsets::LocalPlayer);
    } while (AddressLocalPlayer == NULL);

    Matrix ViewMatrix;
    Vec2 LineOrigin{};

    //Point of Drawing Line Start
    LineOrigin.X = 0.0f;
    LineOrigin.Y = -1.0f;
    //Team Color Dynamic structure
    ColorA* Color = new ColorA();


    bool boolMenuVisible = true;
    bool boolPauseStuff = true;


    //ESP Lines
    bool EspLines = true;
    bool TeamEspLines = true;

    //ESP Boxes
    bool Esp = true;
    bool TeamEsp = true;

   


    // Main loop
    while (!glfwWindowShouldClose(Window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //Show and Hide Window
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            boolMenuVisible = !boolMenuVisible;
            if (boolMenuVisible)
            {
                ShowWindow(Window);
            }
            else
            {
                HideWindow(Window);
            }
        }

        // Draw here
        if (boolMenuVisible)
        {
            ImGui::Checkbox("2D ESP", &Esp);
            ImGui::Checkbox("2D TeamESP", &TeamEsp);
            ImGui::Checkbox("ESPLines", &EspLines);
            ImGui::Checkbox("TeamEspLines", &TeamEspLines);


            if (ImGui::Button("Resume"))
            {
                boolPauseStuff = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Pause"))
            {
                boolPauseStuff = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Exit"))
            {
                return 0;
            }
            
        }

        uintptr_t AddressLocalPlayer;
        if (boolPauseStuff == true)
        {
            do
            {
                AddressLocalPlayer = Mem.ReadMemory<uintptr_t>(HandleOfProcess, ClientBaseAddress + offsets::LocalPlayer);
            } while (AddressLocalPlayer == NULL);

            ViewMatrix = Mem.ReadMemory<Matrix>(HandleOfProcess, ClientBaseAddress + offsets::ViewMatrixOffset);

            int LocalPlayerTeam = Mem.ReadMemory<int>(HandleOfProcess, AddressLocalPlayer + offsets::m_iTeamNum);

            int EntityTeam = 0;

            for (short int i = 0; i < 64; ++i)
            {
                uintptr_t Entity = Mem.ReadMemory<uintptr_t>(HandleOfProcess, ClientBaseAddress + offsets::EntityList + i * 0x10);
                //EntityList
                if (Entity == NULL || Entity == AddressLocalPlayer) continue;
                //Dormant Check
                if (Mem.ReadMemory<bool>(HandleOfProcess, Entity + offsets::Dormant)) continue;
                //Health Check
                if (Mem.ReadMemory<uintptr_t>(HandleOfProcess, Entity + offsets::Health) == 0) continue;
                //EntityTeam Check
                EntityTeam = Mem.ReadMemory<int>(HandleOfProcess, Entity + offsets::m_iTeamNum);


                Vec3 EntityLocation = Mem.ReadMemory<Vec3>(HandleOfProcess, Entity + offsets::vecOrigin);

                Vec3 HeadPosition;
                HeadPosition.X = EntityLocation.X;
                HeadPosition.Y = EntityLocation.Y;
                HeadPosition.Z = EntityLocation.Z + 67.0f;

                Vec3 ScreenCoordinatesLeftFront;
                ScreenCoordinatesLeftFront.X = EntityLocation.X + 15.0f;
                ScreenCoordinatesLeftFront.Y = EntityLocation.Y + 10.0f;
                ScreenCoordinatesLeftFront.Z = EntityLocation.Z;

                Vec3 ScreenCoordinatesRightfront;
                ScreenCoordinatesRightfront.X = EntityLocation.X + 15.0f;
                ScreenCoordinatesRightfront.Y = EntityLocation.Y - 10.0f;
                ScreenCoordinatesRightfront.Z = EntityLocation.Z;

                Vec3 ScreenCoordinatesLeftBehind;
                ScreenCoordinatesLeftBehind.X = EntityLocation.X + 10.0f;
                ScreenCoordinatesLeftBehind.Y = EntityLocation.Y + 10.0f;
                ScreenCoordinatesLeftBehind.Z = EntityLocation.Z;

                Vec3 ScreenCoordinatesRightBehind;
                ScreenCoordinatesRightBehind.X = EntityLocation.X - 10.0f;
                ScreenCoordinatesRightBehind.Y = EntityLocation.Y - 10.0f;
                ScreenCoordinatesRightBehind.Z = EntityLocation.Z;

                Vec3 ScreenCoordinatesCenter;
                ScreenCoordinatesCenter.X = EntityLocation.X;
                ScreenCoordinatesCenter.Y = EntityLocation.Y;
                ScreenCoordinatesCenter.Z = EntityLocation.Z;


                Vec2 HeadCoords;
                Vec2 ScreenCoordsLeftFront;
                Vec2 ScreenCoordsRightFront;
                Vec2 ScreenCoordsLeftBehind;
                Vec2 ScreenCoordsRightBehind;
                Vec2 ScreenCoordsCenter;



                //Players coordinates on screen
                if (!W2S(ScreenCoordinatesLeftFront, ScreenCoordsLeftFront, ViewMatrix.VMatrix)) continue;
                if (!W2S(ScreenCoordinatesRightfront, ScreenCoordsRightFront, ViewMatrix.VMatrix)) continue;
                if (!W2S(ScreenCoordinatesLeftFront, ScreenCoordsLeftBehind, ViewMatrix.VMatrix)) continue;
                if (!W2S(ScreenCoordinatesRightfront, ScreenCoordsRightBehind, ViewMatrix.VMatrix)) continue;
                if (!W2S(ScreenCoordinatesCenter, ScreenCoordsCenter, ViewMatrix.VMatrix)) continue;
                //Player's head on screen
                if (!W2S(HeadPosition, HeadCoords, ViewMatrix.VMatrix)) continue;


                //Players Height and Width 
                float height = HeadCoords.Y - ScreenCoordsCenter.Y;
                float width = height / 3.8f;


                switch (EspLines)
                case true:
                    if (EntityTeam != LocalPlayerTeam)
                    {
                        Color->R = 255.0f;
                        Color->G = 0.0f;
                        Color->B = 0.0f;
                        Color->A = 0.0f;
                        DrawLine(LineOrigin, ScreenCoordsCenter, Color);
                        if (Mem.ReadMemory<uintptr_t>(HandleOfProcess, AddressLocalPlayer + offsets::Health) == 0);
                        {
                            DrawLine(LineOrigin, ScreenCoordsCenter, Color);
                        }
                    }

                    switch (TeamEspLines)
                case true:
                    if (EntityTeam == LocalPlayerTeam)
                    {
                        Color->R = 0.0f;
                        Color->G = 255.0f;
                        Color->B = 0.0f;
                        Color->A = 0.0f;
                        DrawLine(LineOrigin, ScreenCoordsCenter, Color);
                        if (Mem.ReadMemory<uintptr_t>(HandleOfProcess, AddressLocalPlayer + offsets::Health) == 0);
                        {
                            DrawLine(LineOrigin, ScreenCoordsCenter, Color);
                        }
                    }

                    switch (Esp)
                case true:
                    if (EntityTeam != LocalPlayerTeam)
                    {
                        Color->R = 255.0f;
                        Color->G = 0.0f;
                        Color->B = 0.0f;
                        Color->A = 0.0f;
                        Draw2DEspBox(ScreenCoordsLeftFront, ScreenCoordsRightFront, ScreenCoordsLeftBehind, ScreenCoordsRightBehind, ScreenCoordsCenter, width, HeadCoords, Color);
                        if (Mem.ReadMemory<uintptr_t>(HandleOfProcess, AddressLocalPlayer + offsets::Health) == 0);
                        {
                            Draw2DEspBox(ScreenCoordsLeftFront, ScreenCoordsRightFront, ScreenCoordsLeftBehind, ScreenCoordsRightBehind, ScreenCoordsCenter, width, HeadCoords, Color);
                        }
                    }

                    switch (TeamEsp)
                case true:
                    if (EntityTeam == LocalPlayerTeam)
                    {
                        Color->R = 0.0f;
                        Color->G = 255.0f;
                        Color->B = 0.0f;
                        Color->A = 0.0f;
                        Draw2DEspBox(ScreenCoordsLeftFront, ScreenCoordsRightFront, ScreenCoordsLeftBehind, ScreenCoordsRightBehind, ScreenCoordsCenter, width, HeadCoords, Color);
                        if (Mem.ReadMemory<uintptr_t>(HandleOfProcess, AddressLocalPlayer + offsets::Health) == 0);
                        {
                            Draw2DEspBox(ScreenCoordsLeftFront, ScreenCoordsRightFront, ScreenCoordsLeftBehind, ScreenCoordsRightBehind, ScreenCoordsCenter, width, HeadCoords, Color);
                        }
                    }


            }
        }

            // Rendering
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(Window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(Window);
        
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(Window);
    glfwTerminate();

    return 0;
}