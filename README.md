MinecraftClone

Project still under work</br>
currently working on chunk rendering logic</br>

TODO::
texture_atlas -> one large image of textures
block (abstract class)
    -> different type of blocks
        -> Load texture from texture atlas with offset glm::vec2()
        -> air -> no_texture, no_mesh, no aabb_collision</br>

chunk -> contains blocks of 16x16x256</br>
chunk_renderer -> own vbo, cubes, all mashed into single mesh, do culling etc, save neighbouring mesh data for noise algorithms</br>
area -> area of chunks, enables deleting of chunk renderers when user does not need them?</br>
view -> contain information of current view</br>
          -> what chunks of in area are visible to camera</br>
renderer->render view</br>
