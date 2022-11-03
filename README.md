"# MinecraftCloneV2" 

TODO::
empty blocks -> air -> no aabb collision</br>
chunk -> contains blocks of 16x16x256</br>
chunk_renderer -> own vbo, cubes, all mashed into single mesh, do culling etc, save neighbouring mesh data for noise algorithms</br>
area -> area of chunks, enables deleting of chunk renderers when user does not need them?</br>
view -> contain information of current view</br>
          -> what chunks of in area are visible to camera</br>
renderer->render view</br>

everything tickable and updatable</br>
tickable -> every object is "Ticking" and does checks and updates every tick</br>
(optional. tick may be every 3rd frame)</br>
tick everything in while loop first</br>
render afterwards</br>


update is kinda same as tick for now, maybe just delete Updatable from utils.hpp</br>
update()</br>
render()</br>
