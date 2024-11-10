#include "imgui.h"
#include "imgui_impl_raylib.h"
#include "imgui_impl_opengl3.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else
    #define GLSL_VERSION            100
#endif

void InitImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplRaylib_Init();
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();
}

void ShutdownImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplRaylib_Shutdown();
    ImGui::DestroyContext();
}

void DrawImGuiContent()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();

	ImGui::SetNextWindowSize(ImVec2(800, 800));
    ImGui::Begin("ImGui in Raylib Example");
    ImGui::Text("Hello, VR!");
    ImGui::Text("This is an ImGui window rendered to a texture.");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(void)
{
    InitWindow(800, 600, "Raylib + ImGui Render to Texture");
    SetTargetFPS(60);

    InitImGui();

    Camera camera = { 0 };
    camera.position = (Vector3){ 4.0f, 10.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    RenderTexture2D imguiTexture = LoadRenderTexture(1024, 1024);

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_FREE);

        BeginTextureMode(imguiTexture);
            ClearBackground(BLANK);
            DrawImGuiContent();
        EndTextureMode();

		BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
				rlDisableBackfaceCulling();

				rlSetTexture(imguiTexture.texture.id);

				rlBegin(RL_QUADS);
					rlNormal3f(0, 1, 0);

					float yPos = 2.0f;

					rlTexCoord2f(0, 1);
					rlVertex3f(-1.0f, yPos, -1.0f);

					rlTexCoord2f(1, 1);
					rlVertex3f(1.0f, yPos, -1.0f);

					rlTexCoord2f(1, 0);
					rlVertex3f(1.0f, yPos, 1.0f);

					rlTexCoord2f(0, 0);
					rlVertex3f(-1.0f, yPos, 1.0f);
				rlEnd();

				rlSetTexture(0);

				DrawGrid(10, 1.0f);
			EndMode3D();

			DrawText("Use WASD and mouse to move the camera", 10, 10, 20, DARKGRAY);
		EndDrawing();

    }

    UnloadRenderTexture(imguiTexture);
    ShutdownImGui();
    CloseWindow();

    return 0;
}
