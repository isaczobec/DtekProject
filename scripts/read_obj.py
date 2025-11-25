
import math
import os
import sys

def float_to_fp(f: float) -> str:

    floor = math.floor(f*(2**16))
    if (floor < 0):
        floor = -floor
        floor = ~floor + 1

    return hex(floor)

class Vertex():
    def __init__(self, x: float, y: float, z: float):
        self.x = float(x)
        self.y = float(y)
        self.z = float(z)

    def __str__(self):
        return f"{{{float_to_fp(self.x)}, {float_to_fp(self.y)}, {float_to_fp(self.z)}}}"


class UvCoord():
    def __init__(self, u: float, v: float):
        self.u = float(u)
        self.v = float(v)

    def __str__(self):
        return f"{float_to_fp(self.u)}, {float_to_fp(self.v)}"

class Triangle():
    def __init__(self, vi1: int, vi2: int, vi3: int):
        self.vi1 = int(vi1)
        self.vi2 = int(vi2)
        self.vi3 = int(vi3)

    def __str__(self):
        return f"{self.vi1-1}, {self.vi2-1}, {self.vi3-1}"

class Model():
    def __init__(
            self, 
            vertex_data_start: int, 
            vertex_data_count: int,
            index_data_start: int, 
            index_data_count: int,
            ):
        self.vertex_data_start: int = vertex_data_start 
        self.vertex_data_count: int = vertex_data_count
        self.index_data_start: int = index_data_start
        self.index_data_count: int = index_data_count

    def __str__(self):
        return f"{{\n.vertexBuffer = &vertexData[{self.vertex_data_start}],\n.vertexCount = {self.vertex_data_count},\n.indexBuffer = &indexData[{self.index_data_start}],\n.indexBufferCount = {self.index_data_count}  // 12 triangles * 3 vertices each\n}}"
    
VERTEX_DATA_START_LINE = "Vec3 vertexData[VERTEX_DATA_TOTAL_SIZE_BYTES] = {"
VERTEX_DATA_END_LINE = "};"

INDEX_DATA_START_LINE = "uint16 indexData[INDEX_DATA_TOTAL_SIZE_BYTES] = {"
INDEX_DATA_END_LINE = "};"

MODELS_START_LINE = "Model models[MODEL_COUNT] = {"
MODELS_END_LINE = "};"


def read_obj(path: str):

    vertex_ptr = 0
    index_ptr = 0

    models: list[Model] = []

    verts: list[Vertex] = []
    uvs: list[UvCoord] = []
    tris: list[Triangle] = []

    for (dirpath, dirnames, filenames) in os.walk(path):
        for filename in filenames:
            if not ".obj" in filename: continue

            file_path = os.path.join(dirpath,filename)

            lines = []
            with open(file_path, mode="r", encoding="utf8") as file:
                lines = file.readlines()

            v_start = vertex_ptr
            i_start = index_ptr

            for line in lines:
                if "#" in line:
                    continue

                if "v " in line:
                    vals = line.split(" ")[1:]
                    # print(vals)
                    verts.append(Vertex(vals[0],vals[1],vals[2]))
                    vertex_ptr += 1

                if "vt " in line:
                    vals = line.split(" ")[1:]
                    uvs.append(UvCoord(vals[0],vals[1]))

                if "f " in line:
                    vals = line.split(" ")[1:]
                    tris.append(Triangle(
                        vals[0].split("/")[0],
                        vals[1].split("/")[0],
                        vals[2].split("/")[0],
                        ))
                    index_ptr += 3
            
            models.append(Model(
                v_start,
                vertex_ptr - v_start,
                i_start,
                index_ptr - i_start
            ))


    print(VERTEX_DATA_START_LINE)
    for vert in verts:
        print("    ",vert,",",sep="")
    print(VERTEX_DATA_END_LINE)

    print(INDEX_DATA_START_LINE)
    for tri in tris:
        print("    ",tri,",",sep="")
    print(INDEX_DATA_END_LINE)

    print(MODELS_START_LINE)
    for model in models:
        print(model,",",end="")
    print("\n",MODELS_END_LINE,sep="")

def main():
    read_obj(sys.argv[1])

if __name__ == "__main__":
    main()